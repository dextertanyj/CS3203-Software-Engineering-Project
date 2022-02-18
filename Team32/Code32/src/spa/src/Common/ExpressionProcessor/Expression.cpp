#include "Common/ExpressionProcessor/Expression.h"

#include <cassert>

#include "Common/Converter.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/BinaryLogicalNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/ExpressionProcessor/ParenthesesWrapper.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/UnaryLogicalNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "Common/TypeDefs.h"
#include "Common/Validator.h"

using namespace std;
using namespace Common::ExpressionProcessor;

Expression::Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<ConstVal> constants)
	: root(move(root)), variables(move(variables)), constants(move(constants)) {}

Expression Expression::parse(LexerInterface& lex, ExpressionType type) {
	Acceptor acceptor = OperatorAcceptor::getAcceptor(type);
	unordered_set<VarRef> variables;
	unordered_set<ConstVal> constants;
	shared_ptr<ExpressionNode> lhs = parseTerminal(lex, acceptor, variables, constants);
	shared_ptr<ExpressionNode> expression = Expression::construct(lex, acceptor, variables, constants, lhs, 0);
	if (dynamic_pointer_cast<ParenthesesWrapper>(expression) != nullptr) {
		expression = dynamic_pointer_cast<ParenthesesWrapper>(expression)->getExpression();
		if (dynamic_pointer_cast<RelationalNode>(expression) != nullptr || dynamic_pointer_cast<LogicalNode>(expression) != nullptr) {
			throw ExpressionProcessorException("Unsupported nested parentheses");
		}
	}
	if (!checkExpressionType(expression, type)) {
		throw ExpressionProcessorException("Expression invalid");
	}
	return {expression, variables, constants};
}

shared_ptr<ExpressionNode> Expression::construct(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
                                                 unordered_set<ConstVal>& constants, shared_ptr<ExpressionNode> lhs, int precedence) {
	string lookahead = lex.peekToken();
	if (!acceptor(lookahead)) {
		return lhs;
	}

	// Safe to unwrap parentheses wrapper since closing parentheses are not accepted by any acceptor.
	if (dynamic_pointer_cast<ParenthesesWrapper>(lhs) != nullptr) {
		lhs = dynamic_pointer_cast<ParenthesesWrapper>(lhs)->getExpression();
	}

	// Binary logical operators are a special case since they must be fully parenthesized
	if (OperatorAcceptor::acceptBinaryLogical(lookahead)) {
		string token = lex.readToken();
		MathematicalOperator op = Converter::convertMathematical(token);

		assert(op == MathematicalOperator::And || op == MathematicalOperator::Or);

		shared_ptr<LogicalNode> lhs_logical = dynamic_pointer_cast<LogicalNode>(lhs);
		shared_ptr<ExpressionNode> rhs = parseTerminalSafe(lex, acceptor, variables, constants);
		shared_ptr<LogicalNode> rhs_logical = dynamic_pointer_cast<LogicalNode>(rhs);
		if (lhs_logical == nullptr || rhs_logical == nullptr) {
			throw ExpressionProcessorException("Expected conditional expression");
		}
		return make_shared<BinaryLogicalNode>(op, lhs_logical, rhs_logical);
	}

	while (acceptor(lookahead) && getPrecedence(Converter::convertMathematical(lookahead)) >= precedence) {
		string token = lex.readToken();
		MathematicalOperator op = Converter::convertMathematical(token);
		shared_ptr<ExpressionNode> rhs = parseTerminalSafe(lex, acceptor, variables, constants);
		lookahead = lex.peekToken();
		while (acceptor(lookahead) && getPrecedence(Converter::convertMathematical(lookahead)) > getPrecedence(op)) {
			rhs = construct(lex, acceptor, variables, constants, rhs, getPrecedence(Converter::convertMathematical(lookahead)));
			lookahead = lex.peekToken();
		}
		shared_ptr<AtomicNode> lhs_atomic = dynamic_pointer_cast<AtomicNode>(lhs);
		shared_ptr<AtomicNode> rhs_atomic = dynamic_pointer_cast<AtomicNode>(rhs);
		if (lhs_atomic == nullptr || rhs_atomic == nullptr) {
			throw ExpressionProcessorException("Expected atomic expression");
		}
		if (OperatorAcceptor::acceptRelationalStrict(token)) {
			lhs = make_shared<RelationalNode>(op, lhs_atomic, rhs_atomic);
		} else {
			lhs = make_shared<ArithmeticNode>(op, lhs_atomic, rhs_atomic);
		}
	}
	return lhs;
}

shared_ptr<ExpressionNode> Expression::parseTerminalSafe(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
                                                         unordered_set<ConstVal>& constants) {
	shared_ptr<ExpressionNode> expression = parseTerminal(lex, acceptor, variables, constants);
	if (dynamic_pointer_cast<ParenthesesWrapper>(expression) != nullptr) {
		expression = dynamic_pointer_cast<ParenthesesWrapper>(expression)->getExpression();
	}
	return expression;
}

shared_ptr<ExpressionNode> Expression::parseTerminal(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
                                                     unordered_set<ConstVal>& constants) {
	string token = lex.readToken();
	if (OperatorAcceptor::acceptUnaryLogical(token)) {
		if (!acceptor(token)) {
			throw ExpressionProcessorException("Unexpected token received: " + token);
		}
		token = lex.peekToken();  // Do not consume this parenthesis since the subsequent call to parseTerminal will consume it.
		if (token != "(") {
			throw ExpressionProcessorException("Unexpected token received: " + token);
		}
		shared_ptr<ExpressionNode> expression = parseTerminalSafe(lex, acceptor, variables, constants);

		shared_ptr<LogicalNode> logical_expression = dynamic_pointer_cast<LogicalNode>(expression);
		if (logical_expression == nullptr) {
			throw ExpressionProcessorException("Expected conditional expression");
		}
		// No check for ')' since we asserted there is an '(' and parseTerminal will throw an error for mismatched parentheses.
		return make_shared<UnaryLogicalNode>(MathematicalOperator::Not, logical_expression);
	}
	if (token == "(") {
		// We only have to check for disallowed nested parentheses when parseTerminal is called sequentially without any actual
		// construction.
		shared_ptr<ExpressionNode> lhs = parseTerminal(lex, acceptor, variables, constants);
		shared_ptr<ExpressionNode> expression = construct(lex, acceptor, variables, constants, lhs, 0);
		if (dynamic_pointer_cast<ParenthesesWrapper>(expression) != nullptr) {
			expression = dynamic_pointer_cast<ParenthesesWrapper>(expression)->getExpression();
			if (dynamic_pointer_cast<RelationalNode>(expression) != nullptr || dynamic_pointer_cast<LogicalNode>(expression) != nullptr) {
				throw ExpressionProcessorException("Unsupported nested parentheses");
			}
		}
		token = lex.readToken();
		if (token != ")") {
			throw ExpressionProcessorException("Unexpected token received: " + token);
		}
		return make_shared<ParenthesesWrapper>(expression);
	}
	if (Validator::validateName(token)) {
		variables.insert(token);
		return make_shared<VariableNode>(token);
	}
	if (Validator::validateInteger(token)) {
		constants.insert(Converter::convertInteger(token));
		return make_shared<ConstantNode>(token);
	}
	throw ExpressionProcessorException("Unknown token received: " + token);
}

int Expression::getPrecedence(MathematicalOperator op) {
	// Logical operators are expected to be fully parenthesized and so should never be chained.
	if (op == MathematicalOperator::And || op == MathematicalOperator::Or) {
		return -1;
	}
	if (op == MathematicalOperator::LTE || op == MathematicalOperator::GTE || op == MathematicalOperator::LT ||
	    op == MathematicalOperator::GT || op == MathematicalOperator::EQ || op == MathematicalOperator::NEQ) {
		return 1;
	}
	if (op == MathematicalOperator::Plus || op == MathematicalOperator::Minus) {
		return 2;
	}
	return 3;
}

bool Expression::checkExpressionType(const shared_ptr<ExpressionNode>& expression, ExpressionType type) {
	switch (type) {
		case ExpressionType::Arithmetic:
			return dynamic_pointer_cast<AtomicNode>(expression) != nullptr;
		case ExpressionType::Relational:
			return dynamic_pointer_cast<RelationalNode>(expression) != nullptr;
		case ExpressionType::Logical:
			return dynamic_pointer_cast<LogicalNode>(expression) != nullptr || dynamic_pointer_cast<RelationalNode>(expression) != nullptr;
		default:
			throw ExpressionProcessorException("Unknown expression type received");
	}
}

bool Expression::contains(const Expression& other) { return root->contains(other.root); }

bool Expression::equals(const Expression& other) { return root->equals(other.root); }

unordered_set<ConstVal> Expression::getConstants() { return constants; }
unordered_set<VarRef> Expression::getVariables() { return variables; }

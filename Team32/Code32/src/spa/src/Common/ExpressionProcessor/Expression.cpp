#include "Common/ExpressionProcessor/Expression.h"

#include <cassert>
#include <string>

#include "Common/Converter.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/BinaryLogicalNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/UnaryLogicalNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "Common/TypeDefs.h"
#include "Common/Validator.h"

using namespace std;
using namespace Common::ExpressionProcessor;

Expression::Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<int> constants)
	: root(std::move(root)), variables(std::move(variables)), constants(std::move(constants)) {}

Expression Expression::parse(LexerInterface& lex, bool (*acceptor)(string op)) {
	unordered_set<VarRef> variables;
	unordered_set<int> constants;
	shared_ptr<ExpressionNode> expression =
		Expression::construct(lex, acceptor, variables, constants, parseTerminal(lex, acceptor, variables, constants), 0);
	return Expression(expression, variables, constants);
}

shared_ptr<ExpressionNode> Expression::construct(LexerInterface& lex, bool (*acceptor)(string op), unordered_set<VarRef>& variables,
                                                 unordered_set<int>& constants, shared_ptr<ExpressionNode> lhs, int precedence) {
	string lookahead = lex.peekToken();
	if (!acceptor(lookahead)) {
		return lhs;
	}

	if (OperatorAcceptor::acceptBinaryLogical(lookahead)) {
		string token = lex.readToken();
		MathematicalOperator op = Converter::convertMathematical(token);

		assert(op == MathematicalOperator::And || op == MathematicalOperator::Or);

		shared_ptr<LogicalNode> lhs_logical = dynamic_pointer_cast<LogicalNode>(lhs);
		shared_ptr<ExpressionNode> rhs = parseTerminal(lex, acceptor, variables, constants);
		shared_ptr<LogicalNode> rhs_logical = dynamic_pointer_cast<LogicalNode>(rhs);
		if (lhs_logical == nullptr || rhs_logical == nullptr) {
			throw ExpressionProcessorException("Expected conditional expression" + token);
		}
		return make_shared<BinaryLogicalNode>(op, lhs_logical, rhs_logical);
	}

	while (acceptor(lookahead) && getPrecedence(Converter::convertMathematical(lookahead)) >= precedence) {
		string token = lex.readToken();
		MathematicalOperator op = Converter::convertMathematical(token);
		shared_ptr<ExpressionNode> rhs = parseTerminal(lex, acceptor, variables, constants);
		lookahead = lex.peekToken();
		while (Validator::validateArithmeticOperator(lookahead) &&
		       getPrecedence(Converter::convertMathematical(lookahead)) > getPrecedence(op)) {
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

shared_ptr<ExpressionNode> Expression::parseTerminal(LexerInterface& lex, bool (*acceptor)(string op), unordered_set<VarRef>& variables,
                                                     unordered_set<int>& constants) {
	string token = lex.readToken();
	if (OperatorAcceptor::acceptUnaryLogical(token)) {
		if (!acceptor(token)) {
			throw ExpressionProcessorException("Unexpected token received: " + token);
		}
		token = lex.peekToken();
		if (token != "(") {
			throw ExpressionProcessorException("Unexpected token received: " + token);
		}
		shared_ptr<ExpressionNode> expression = parseTerminal(lex, acceptor, variables, constants);
		shared_ptr<LogicalNode> logical_expression = dynamic_pointer_cast<LogicalNode>(expression);
		if (logical_expression == nullptr) {
			throw ExpressionProcessorException("Expected conditional expression");
		}
		// No check for ')' since we asserted there is an '(' and parseTerminal will throw an error for mismatched parentheses.
		return make_shared<UnaryLogicalNode>(MathematicalOperator::Not, logical_expression);
	}
	if (token == "(") {
		shared_ptr<ExpressionNode> lhs = parseTerminal(lex, acceptor, variables, constants);
		shared_ptr<ExpressionNode> expression = construct(lex, acceptor, variables, constants, lhs, 0);
		token = lex.readToken();
		if (token != ")") {
			throw ExpressionProcessorException("Unexpected token received: " + token);
		}
		return expression;
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
	if (op == MathematicalOperator::LTE || op == MathematicalOperator::GTE || op == MathematicalOperator::LT ||
	    op == MathematicalOperator::GT || op == MathematicalOperator::EQ || op == MathematicalOperator::NEQ) {
		return 1;
	}
	if (op == MathematicalOperator::Plus || op == MathematicalOperator::Minus) {
		return 2;
	}
	return 3;
}

bool Expression::contains(const Expression& other) { return root->contains(other.root); }

bool Expression::equals(const Expression& other) { return root->equals(other.root); }

unordered_set<int> Expression::getConstants() { return constants; }

unordered_set<VarRef> Expression::getVariables() { return variables; }

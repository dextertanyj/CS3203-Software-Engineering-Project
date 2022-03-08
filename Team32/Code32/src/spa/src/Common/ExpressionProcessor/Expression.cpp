#include "Common/ExpressionProcessor/Expression.h"

#include <cassert>
#include <utility>

#include "Common/Converter.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/BinaryLogicalNode.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/ExpressionProcessor/ParenthesesWrapper.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "Common/ExpressionProcessor/UnaryLogicalNode.h"
#include "Common/TypeDefs.h"
#include "Common/Validator.h"

#define LOGICAL_PRECEDENCE -1
#define RELATIONAL_PRECEDENCE 1
#define ADD_SUBTRACT_PRECEDENCE 2
#define MULTIPLY_DIVIDE_MODULUS_PRECEDENCE 3

using namespace Common::ExpressionProcessor;

struct ExtractVisitor {
	shared_ptr<ExpressionNode> expression;
	bool wrapped = false;
	void operator()(ParenthesesWrapper parentheses) {
		shared_ptr<ExpressionNode> expr = parentheses.getExpression();
		wrapped = true;
		expression = move(expr);
	}
	void operator()(shared_ptr<ExpressionNode> expr) { expression = move(expr); }
};

Expression::Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<ConstVal> constants)
	: root(move(root)), variables(move(variables)), constants(move(constants)) {}

Expression Expression::parse(LexerInterface& lex, ExpressionType type) {
	Acceptor acceptor = OperatorAcceptor::getAcceptor(type);
	unordered_set<VarRef> variables;
	unordered_set<ConstVal> constants;
	variant<ParenthesesWrapper, shared_ptr<ExpressionNode>> lhs = parseTerminal(lex, acceptor, variables, constants);
	ExtractVisitor extract_visitor;
	visit(extract_visitor, lhs);
	shared_ptr<ExpressionNode> expression = Expression::construct(lex, acceptor, variables, constants, extract_visitor.expression, 0);
	if (extract_visitor.wrapped && expression == extract_visitor.expression) {
		if (dynamic_pointer_cast<RelationalNode>(expression) != nullptr || dynamic_pointer_cast<LogicalNode>(expression) != nullptr) {
			throw ExpressionProcessorException("Unsupported nested parentheses in expression.");
		}
	}
	if (!checkExpressionType(expression, type)) {
		throw ExpressionProcessorException("Invalid expression type received.");
	}
	return {expression, variables, constants};
}

shared_ptr<ExpressionNode> Expression::construct(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
                                                 unordered_set<ConstVal>& constants, shared_ptr<ExpressionNode> lhs, int precedence) {
	string lookahead = lex.peekToken();
	if (!acceptor(lookahead)) {
		return lhs;
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
			throw ExpressionProcessorException("Expected conditional expression.");
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
			throw ExpressionProcessorException("Expected atomic expression.");
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
	variant<ParenthesesWrapper, shared_ptr<ExpressionNode>> expression = parseTerminal(lex, acceptor, variables, constants);
	ExtractVisitor extract_visitor;
	visit(extract_visitor, expression);
	return extract_visitor.expression;
}

variant<ParenthesesWrapper, shared_ptr<ExpressionNode>> Expression::parseTerminal(LexerInterface& lex, Acceptor acceptor,
                                                                                  unordered_set<VarRef>& variables,
                                                                                  unordered_set<ConstVal>& constants) {
	string token = lex.readToken();
	if (OperatorAcceptor::acceptUnaryLogical(token)) {
		if (!acceptor(token)) {
			throw ExpressionProcessorException("Unexpected token received: " + token + ".");
		}
		token = lex.peekToken();  // Do not consume this parenthesis since the subsequent call to parseTerminal will consume it.
		if (token != "(") {
			throw ExpressionProcessorException("Unexpected token received: " + token + ".");
		}
		shared_ptr<ExpressionNode> expression = parseTerminalSafe(lex, acceptor, variables, constants);

		shared_ptr<LogicalNode> logical_expression = dynamic_pointer_cast<LogicalNode>(expression);
		if (logical_expression == nullptr) {
			throw ExpressionProcessorException("Expected conditional expression.");
		}
		// No check for ')' since we asserted there is an '(' and parseTerminal will throw an error for mismatched parentheses.
		return make_shared<UnaryLogicalNode>(MathematicalOperator::Not, logical_expression);
	}
	if (token == "(") {
		// We only have to check for disallowed nested parentheses when parseTerminal is called sequentially without any actual
		// construction.
		variant<ParenthesesWrapper, shared_ptr<ExpressionNode>> lhs = parseTerminal(lex, acceptor, variables, constants);
		ExtractVisitor extract_visitor;
		visit(extract_visitor, lhs);
		shared_ptr<ExpressionNode> expression = construct(lex, acceptor, variables, constants, extract_visitor.expression, 0);
		if (extract_visitor.wrapped && expression == extract_visitor.expression) {
			if (dynamic_pointer_cast<RelationalNode>(expression) != nullptr || dynamic_pointer_cast<LogicalNode>(expression) != nullptr) {
				throw ExpressionProcessorException("Unsupported nested parentheses in expression.");
			}
		}
		token = lex.readToken();
		if (token != ")") {
			throw ExpressionProcessorException("Unexpected token received: " + token + ".");
		}
		return ParenthesesWrapper(expression);
	}
	if (Validator::validateName(token)) {
		variables.insert(token);
		return make_shared<TerminalNode<string>>(token);
	}
	if (Validator::validateInteger(token)) {
		ConstVal value = Converter::convertInteger(token);
		constants.insert(value);
		return make_shared<TerminalNode<ConstVal>>(value);
	}
	throw ExpressionProcessorException("Unexpected token received: " + token + ".");
}

int Expression::getPrecedence(MathematicalOperator op) {
	// Logical operators are expected to be fully parenthesized and so should never be chained.
	if (op == MathematicalOperator::And || op == MathematicalOperator::Or) {
		return LOGICAL_PRECEDENCE;
	}
	if (op == MathematicalOperator::LTE || op == MathematicalOperator::GTE || op == MathematicalOperator::LT ||
	    op == MathematicalOperator::GT || op == MathematicalOperator::EQ || op == MathematicalOperator::NEQ) {
		return RELATIONAL_PRECEDENCE;
	}
	if (op == MathematicalOperator::Plus || op == MathematicalOperator::Minus) {
		return ADD_SUBTRACT_PRECEDENCE;
	}
	return MULTIPLY_DIVIDE_MODULUS_PRECEDENCE;
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
			throw ExpressionProcessorException("Unknown expression type received.");
	}
}

bool Expression::contains(const Expression& other) { return root->contains(other.root); }

bool Expression::equals(const Expression& other) { return root->equals(other.root); }

unordered_set<ConstVal> Expression::getConstants() { return constants; }
unordered_set<VarRef> Expression::getVariables() { return variables; }

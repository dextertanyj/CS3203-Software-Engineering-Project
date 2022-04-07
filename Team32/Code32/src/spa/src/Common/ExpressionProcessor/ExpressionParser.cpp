#include "Common/ExpressionProcessor/ExpressionParser.h"

#include "Common/Converter.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/BinaryLogicalNode.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/ExpressionProcessor/ParenthesesWrapper.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "Common/ExpressionProcessor/UnaryLogicalNode.h"
#include "Common/Validator.h"

#define LOGICAL_PRECEDENCE (-1)
#define START_PRECEDENCE 0
#define RELATIONAL_PRECEDENCE 1
#define ADD_SUBTRACT_PRECEDENCE 2
#define MULTIPLY_DIVIDE_MODULUS_PRECEDENCE 3
#define OPEN_PARENTHESES "("
#define CLOSE_PARENTHESES ")"

using namespace std;
using namespace Common::ExpressionProcessor;

ExpressionParser::ExpressionParser(LexerInterface& lex, ExpressionType type) : lex(lex), type(type) {}

Expression ExpressionParser::parse() {
	Acceptor acceptor = OperatorAcceptor::getAcceptor(type);
	ParenthesizedExpression lhs = parseTerminal(acceptor);
	ParenthesizedExpression expression = construct(acceptor, lhs, START_PRECEDENCE);
	ParenthesesWrapper test_wrap = ParenthesesWrapper(expression);  // Test if outermost expression has an extra set of parentheses.
	if (!checkExpressionType(test_wrap.getExpression(), type)) {
		throw ExpressionProcessorException("Incorrect expression type parsed.");
	}
	return {test_wrap.getExpression(), variables, constants};
}

/**
 * This function applies the operator precedence parsing algorithm.
 */
ParenthesizedExpression ExpressionParser::construct(Acceptor acceptor, ParenthesizedExpression lhs, int precedence) {
	string lookahead = lex.peekToken();
	if (!acceptor(lookahead)) {
		return lhs;
	}

	// Binary logical operators are a special case since they must be fully parenthesized.
	if (OperatorAcceptor::acceptBinaryLogical(lookahead)) {
		return parseBinaryLogical(lhs);
	}

	while (acceptor(lookahead) && getPrecedence(Converter::convertMathematical(lookahead)) >= precedence) {
		string token = lex.readToken();
		MathematicalOperator opr = Converter::convertMathematical(token);
		shared_ptr<ExpressionNode> rhs = parseTerminalSafe(acceptor);
		lookahead = lex.peekToken();
		while (acceptor(lookahead) && getPrecedence(Converter::convertMathematical(lookahead)) > getPrecedence(opr)) {
			rhs = getExpression(construct(acceptor, rhs, getPrecedence(Converter::convertMathematical(lookahead))));
			lookahead = lex.peekToken();
		}
		shared_ptr<AtomicNode> lhs_atomic = dynamic_pointer_cast<AtomicNode>(getExpression(lhs));
		shared_ptr<AtomicNode> rhs_atomic = dynamic_pointer_cast<AtomicNode>(getExpression(rhs));
		if (lhs_atomic == nullptr || rhs_atomic == nullptr) {
			throw ExpressionProcessorException("Expected atomic expression.");
		}
		if (OperatorAcceptor::acceptRelationalStrict(token)) {
			lhs = make_shared<RelationalNode>(opr, lhs_atomic, rhs_atomic);
		} else {
			lhs = make_shared<ArithmeticNode>(opr, lhs_atomic, rhs_atomic);
		}
	}
	return lhs;
}

ParenthesizedExpression ExpressionParser::parseTerminal(Acceptor acceptor) {
	string token = lex.readToken();
	if (OperatorAcceptor::acceptUnaryLogical(token)) {
		if (!acceptor(token)) {
			throw ExpressionProcessorException("Unexpected token received: " + token + ".");
		}
		return parseUnaryLogical();
	}
	if (token == OPEN_PARENTHESES) {
		// We only have to check for disallowed nested parentheses when parseTerminal is called sequentially without any actual
		// construction.
		ParenthesizedExpression lhs = parseTerminal(acceptor);
		ParenthesizedExpression expression = construct(acceptor, lhs, START_PRECEDENCE);
		lex.match(CLOSE_PARENTHESES);
		return ParenthesesWrapper(expression);
	}
	if (Validator::validateName(token)) {
		variables.emplace(token);
		return make_shared<TerminalNode<VarRef>>(token);
	}
	if (Validator::validateInteger(token)) {
		ConstVal value = Converter::convertInteger(token);
		constants.emplace(value);
		return make_shared<TerminalNode<ConstVal>>(value);
	}
	throw ExpressionProcessorException("Unexpected token received: " + token + ".");
}

shared_ptr<ExpressionNode> ExpressionParser::parseTerminalSafe(Acceptor acceptor) {
	ParenthesizedExpression expression = parseTerminal(acceptor);
	return getExpression(expression);
}

shared_ptr<UnaryLogicalNode> ExpressionParser::parseUnaryLogical() {
	lex.check(OPEN_PARENTHESES);
	shared_ptr<ExpressionNode> expression = parseTerminalSafe(OperatorAcceptor::acceptLogical);

	shared_ptr<LogicalNode> logical_expression = dynamic_pointer_cast<LogicalNode>(expression);
	if (logical_expression == nullptr) {
		throw ExpressionProcessorException("Expected conditional expression.");
	}
	// No check for ')' since we asserted there is an '(' and parseTerminal will throw an error for mismatched parentheses.
	return make_shared<UnaryLogicalNode>(MathematicalOperator::Not, logical_expression);
}

shared_ptr<BinaryLogicalNode> ExpressionParser::parseBinaryLogical(const ParenthesizedExpression& lhs) {
	string token = lex.readToken();
	MathematicalOperator op = Converter::convertMathematical(token);
	ParenthesizedExpression rhs = parseTerminal(OperatorAcceptor::acceptLogical);
	shared_ptr<LogicalNode> lhs_logical = dynamic_pointer_cast<LogicalNode>(getExpression(lhs));
	shared_ptr<LogicalNode> rhs_logical = dynamic_pointer_cast<LogicalNode>(getExpression(rhs));
	if (lhs_logical == nullptr || rhs_logical == nullptr) {
		throw ExpressionProcessorException("Expected conditional expression.");
	}
	return make_shared<BinaryLogicalNode>(op, lhs_logical, rhs_logical);
}

shared_ptr<ExpressionNode> ExpressionParser::getExpression(const ParenthesizedExpression& expression) {
	shared_ptr<ExpressionNode> result;
	visit(Visitor{[&](const shared_ptr<ExpressionNode>& expr) { result = expr; },
	              [&](const ParenthesesWrapper& wrapper) { result = wrapper.getExpression(); }},
	      expression);
	return result;
}

int ExpressionParser::getPrecedence(MathematicalOperator opr) {
	// Logical operators are expected to be fully parenthesized and so should never be chained.
	if (opr == MathematicalOperator::And || opr == MathematicalOperator::Or) {
		return LOGICAL_PRECEDENCE;
	}
	if (opr == MathematicalOperator::LTE || opr == MathematicalOperator::GTE || opr == MathematicalOperator::LT ||
	    opr == MathematicalOperator::GT || opr == MathematicalOperator::EQ || opr == MathematicalOperator::NEQ) {
		return RELATIONAL_PRECEDENCE;
	}
	if (opr == MathematicalOperator::Plus || opr == MathematicalOperator::Minus) {
		return ADD_SUBTRACT_PRECEDENCE;
	}
	return MULTIPLY_DIVIDE_MODULUS_PRECEDENCE;
}

bool ExpressionParser::checkExpressionType(const shared_ptr<ExpressionNode>& expression, ExpressionType type) {
	switch (type) {
		case ExpressionType::Arithmetic:
			return dynamic_pointer_cast<AtomicNode>(expression) != nullptr;
		case ExpressionType::Relational:
			return dynamic_pointer_cast<RelationalNode>(expression) != nullptr;
		case ExpressionType::Logical:
			return dynamic_pointer_cast<LogicalNode>(expression) != nullptr;
	}
}
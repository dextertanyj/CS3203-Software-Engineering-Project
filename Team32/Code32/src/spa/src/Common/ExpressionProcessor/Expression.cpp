#include "Common/ExpressionProcessor/Expression.h"

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

#define LOGICAL_PRECEDENCE (-1)
#define RELATIONAL_PRECEDENCE 1
#define ADD_SUBTRACT_PRECEDENCE 2
#define MULTIPLY_DIVIDE_MODULUS_PRECEDENCE 3

using namespace Common::ExpressionProcessor;

Expression Expression::parse(LexerInterface& lex, ExpressionType type) {
	Acceptor acceptor = OperatorAcceptor::getAcceptor(type);
	unordered_set<VarRef> variables;
	unordered_set<ConstVal> constants;
	ParenthesizedExpression lhs = parseTerminal(lex, acceptor, variables, constants);
	ParenthesizedExpression expression = construct(lex, acceptor, variables, constants, lhs, 0);
	ParenthesesWrapper test_wrap = ParenthesesWrapper(expression);  // Test if outermost expression has an extra set of parentheses.
	if (!checkExpressionType(test_wrap.getExpression(), type)) {
		throw ExpressionProcessorException("Incorrect expression type parsed.");
	}
	return {test_wrap.getExpression(), variables, constants};
}

Expression::Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<ConstVal> constants)
	: root(move(root)), variables(move(variables)), constants(move(constants)) {
	if (this->root != nullptr) {
		traversal = this->root->traversal();
	}
}

bool Expression::contains(const Expression& other) { return traversal.find(other.traversal) != std::string::npos; }

bool Expression::equals(const Expression& other) { return traversal == other.traversal; }

unordered_set<ConstVal> Expression::getConstants() { return constants; }

unordered_set<VarRef> Expression::getVariables() { return variables; }

ParenthesizedExpression Expression::construct(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
                                              unordered_set<ConstVal>& constants, ParenthesizedExpression lhs, int precedence) {
	string lookahead = lex.peekToken();
	if (!acceptor(lookahead)) {
		return lhs;
	}

	// Binary logical operators are a special case since they must be fully parenthesized
	if (OperatorAcceptor::acceptBinaryLogical(lookahead)) {
		string token = lex.readToken();
		MathematicalOperator op = Converter::convertMathematical(token);

		assert(op == MathematicalOperator::And || op == MathematicalOperator::Or);
		ParenthesizedExpression rhs = parseTerminal(lex, acceptor, variables, constants);
		shared_ptr<LogicalNode> lhs_logical = dynamic_pointer_cast<LogicalNode>(getExpression(lhs));
		shared_ptr<LogicalNode> rhs_logical = dynamic_pointer_cast<LogicalNode>(getExpression(rhs));
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
			rhs = getExpression(
				construct(lex, acceptor, variables, constants, rhs, getPrecedence(Converter::convertMathematical(lookahead))));
			lookahead = lex.peekToken();
		}
		shared_ptr<AtomicNode> lhs_atomic = dynamic_pointer_cast<AtomicNode>(getExpression(lhs));
		shared_ptr<AtomicNode> rhs_atomic = dynamic_pointer_cast<AtomicNode>(getExpression(rhs));
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

ParenthesizedExpression Expression::parseTerminal(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
                                                  unordered_set<ConstVal>& constants) {
	string token = lex.readToken();
	if (OperatorAcceptor::acceptUnaryLogical(token)) {
		if (!acceptor(token)) {
			throw ExpressionProcessorException("Unexpected token received: " + token + ".");
		}
		return parseUnaryLogical(lex, acceptor, variables, constants);
	}
	if (token == "(") {
		// We only have to check for disallowed nested parentheses when parseTerminal is called sequentially without any actual
		// construction.
		ParenthesizedExpression lhs = parseTerminal(lex, acceptor, variables, constants);
		ParenthesizedExpression expression = construct(lex, acceptor, variables, constants, lhs, 0);
		lex.match(")");
		return ParenthesesWrapper(expression);
	}
	if (Validator::validateName(token)) {
		variables.insert(token);
		return make_shared<TerminalNode<VarRef>>(token);
	}
	if (Validator::validateInteger(token)) {
		ConstVal value = Converter::convertInteger(token);
		constants.insert(value);
		return make_shared<TerminalNode<ConstVal>>(value);
	}
	throw ExpressionProcessorException("Unexpected token received: " + token + ".");
}

shared_ptr<ExpressionNode> Expression::parseTerminalSafe(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
                                                         unordered_set<ConstVal>& constants) {
	ParenthesizedExpression expression = parseTerminal(lex, acceptor, variables, constants);
	return getExpression(expression);
}

shared_ptr<UnaryLogicalNode> Expression::parseUnaryLogical(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
                                                           unordered_set<ConstVal>& constants) {
	lex.check("(");
	shared_ptr<ExpressionNode> expression = parseTerminalSafe(lex, acceptor, variables, constants);

	shared_ptr<LogicalNode> logical_expression = dynamic_pointer_cast<LogicalNode>(expression);
	if (logical_expression == nullptr) {
		throw ExpressionProcessorException("Expected conditional expression.");
	}
	// No check for ')' since we asserted there is an '(' and parseTerminal will throw an error for mismatched parentheses.
	return make_shared<UnaryLogicalNode>(MathematicalOperator::Not, logical_expression);
}

shared_ptr<ExpressionNode> Expression::getExpression(const ParenthesizedExpression& expression) {
	shared_ptr<ExpressionNode> result;
	visit(Visitor{[&](const shared_ptr<ExpressionNode>& expr) { result = expr; },
	              [&](const ParenthesesWrapper& wrapper) { result = wrapper.getExpression(); }},
	      expression);
	return result;
}

int Expression::getPrecedence(MathematicalOperator opr) {
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

bool Expression::checkExpressionType(const shared_ptr<ExpressionNode>& expression, ExpressionType type) {
	switch (type) {
		case ExpressionType::Arithmetic:
			return dynamic_pointer_cast<AtomicNode>(expression) != nullptr;
		case ExpressionType::Relational:
			return dynamic_pointer_cast<RelationalNode>(expression) != nullptr;
		case ExpressionType::Logical:
			return dynamic_pointer_cast<LogicalNode>(expression) != nullptr;
	}
}

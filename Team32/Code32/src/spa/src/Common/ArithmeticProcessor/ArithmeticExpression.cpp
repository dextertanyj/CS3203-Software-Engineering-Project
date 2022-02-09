#include "Common/ArithmeticProcessor/ArithmeticExpression.h"

#include <string>

#include "Common/ArithmeticProcessor/ConstantNode.h"
#include "Common/ArithmeticProcessor/BinaryOperatorNode.h"
#include "Common/ArithmeticProcessor/VariableNode.h"
#include "Common/Converter.h"
#include "Common/TypeDefs.h"
#include "Common/Validator.h"

using namespace std;
using namespace Common::ArithmeticProcessor;

ArithmeticExpression::ArithmeticExpression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<int> constants)
	: root(std::move(root)), variables(std::move(variables)), constants(std::move(constants)) {}

ArithmeticExpression ArithmeticExpression::parse(LexerInterface& lex) {
	unordered_set<VarRef> variables;
	unordered_set<int> constants;
	shared_ptr<ExpressionNode> expression =
		ArithmeticExpression::construct(lex, variables, constants, parseTerminal(lex, variables, constants), 0);
	return ArithmeticExpression(expression, variables, constants);
}

shared_ptr<ExpressionNode> ArithmeticExpression::construct(LexerInterface& lex, unordered_set<VarRef>& variables,
                                                           unordered_set<int>& constants, shared_ptr<ExpressionNode> lhs, int precedence) {
	string lookahead = lex.peekToken();
	while (Validator::validateArithmeticOperator(lookahead) && getPrecedence(Converter::convertArithmetic(lookahead)) >= precedence) {
		ArithmeticOperator op = Converter::convertArithmetic(lex.readToken());
		shared_ptr<ExpressionNode> rhs = parseTerminal(lex, variables, constants);
		lookahead = lex.peekToken();
		while (Validator::validateArithmeticOperator(lookahead) &&
		       getPrecedence(Converter::convertArithmetic(lookahead)) > getPrecedence(op)) {
			rhs = construct(lex, variables, constants, rhs, getPrecedence(Converter::convertArithmetic(lookahead)));
			lookahead = lex.peekToken();
		}
		lhs = make_shared<BinaryOperatorNode>(op, lhs, rhs);
	}
	return lhs;
}

shared_ptr<ExpressionNode> ArithmeticExpression::parseTerminal(LexerInterface& lex, unordered_set<VarRef>& variables,
                                                               unordered_set<int>& constants) {
	string token = lex.readToken();
	if (token == "(") {
		shared_ptr<ExpressionNode> lhs = parseTerminal(lex, variables, constants);
		shared_ptr<ExpressionNode> expression = construct(lex, variables, constants, lhs, 0);
		if (lex.readToken() != ")") {
			throw ArithmeticProcessorException("Unexpected token received");
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
	throw ArithmeticProcessorException("Unknown token received");
}

int ArithmeticExpression::getPrecedence(ArithmeticOperator op) {
	if (op == ArithmeticOperator::Plus || op == ArithmeticOperator::Minus) {
		return 1;
	}
	return 2;
}

bool ArithmeticExpression::contains(const ArithmeticExpression& other) { return root->contains(other.root); }

bool ArithmeticExpression::equals(const ArithmeticExpression& other) { return root->equals(other.root); }

unordered_set<int> ArithmeticExpression::getConstants() { return constants; }

unordered_set<VarRef> ArithmeticExpression::getVariables() { return variables; }

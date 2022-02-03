#include "Common/ArithmeticProcessor/ArithmeticExpression.h"

#include <memory>
#include <string>

#include "Common/ArithmeticProcessor/ConstantNode.h"
#include "Common/ArithmeticProcessor/ExpressionNode.h"
#include "Common/ArithmeticProcessor/OperatorNode.h"
#include "Common/ArithmeticProcessor/VariableNode.h"
#include "Common/Converter.h"
#include "Common/TypeDefs.h"
#include "Common/Validator.h"

using namespace std;
using namespace ArithmeticProcessor;
using namespace Converter;
using namespace Validator;

ArithmeticExpression::ArithmeticExpression(shared_ptr<ExpressionNode> root) : root(std::move(root)) {}

ArithmeticExpression ArithmeticExpression::parse(Lexer& lex) {
	shared_ptr<ExpressionNode> expression = ArithmeticExpression::construct(lex, parseTerminal(lex), 0);
	return ArithmeticExpression(expression);
}

shared_ptr<ExpressionNode> ArithmeticExpression::construct(Lexer& lex, shared_ptr<ExpressionNode> lhs, int precedence) {
	string lookahead = lex.peekToken();
	while (validateArithmeticOperator(lookahead) && getPrecedence(convertArithmetic(lookahead)) >= precedence) {
		ArithmeticOperator op = convertArithmetic(lex.readToken());
		shared_ptr<ExpressionNode> rhs = parseTerminal(lex);
		lookahead = lex.peekToken();
		while (validateArithmeticOperator(lookahead) && getPrecedence(convertArithmetic(lookahead)) > getPrecedence(op)) {
			rhs = construct(lex, rhs, getPrecedence(convertArithmetic(lookahead)));
			lookahead = lex.peekToken();
		}
		lhs = make_shared<OperatorNode>(op, lhs, rhs);
	}
	return lhs;
}

shared_ptr<ExpressionNode> ArithmeticExpression::parseTerminal(Lexer& lex) {
	string token = lex.readToken();
	if (token == "(") {
		shared_ptr<ExpressionNode> lhs = parseTerminal(lex);
		shared_ptr<ExpressionNode> expression = construct(lex, lhs, 0);
		if (lex.peekToken() != ")") {
			throw lex.readToken();
		}
		lex.readToken();
		return expression;
	}
	if (validateName(token)) {
		return make_shared<VariableNode>(token);
	}
	if (validateInteger(token)) {
		return make_shared<ConstantNode>(token);
	}
	throw token;
}

int ArithmeticExpression::getPrecedence(ArithmeticOperator op) {
	if (op == ArithmeticOperator::Plus || op == ArithmeticOperator::Minus) {
		return 1;
	}
	return 2;
}

bool ArithmeticExpression::contains(const ArithmeticExpression& other) { return root->contains(other.root); }

bool ArithmeticExpression::equals(const ArithmeticExpression& other) { return root->equals(other.root); }

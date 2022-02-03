#ifndef SPA_ARITHMETICEXPRESSION_H
#define SPA_ARITHMETICEXPRESSION_H

#include <memory>

#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "Common/ArithmeticProcessor/Lexer.h"
#include "Common/TypeDefs.h"

using namespace std;

class ArithmeticProcessor::ArithmeticExpression {
public:
	static ArithmeticExpression parse(ArithmeticProcessor::Lexer& lex);
	explicit ArithmeticExpression(shared_ptr<ExpressionNode> root);
	bool equals(const ArithmeticExpression& other);
	bool contains(const ArithmeticExpression& other);

private:
	shared_ptr<ExpressionNode> root;
	static shared_ptr<ExpressionNode> construct(Lexer& lex, shared_ptr<ExpressionNode> lhs, int precedence);
	static shared_ptr<ExpressionNode> parseTerminal(Lexer& lex);
	static int getPrecedence(ArithmeticOperator op);
};

#endif  // SPA_ARITHMETICEXPRESSION_H

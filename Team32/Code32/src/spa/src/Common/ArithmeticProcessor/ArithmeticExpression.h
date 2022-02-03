#ifndef SPA_ARITHMETICEXPRESSION_H
#define SPA_ARITHMETICEXPRESSION_H

#include <memory>
#include <unordered_set>

#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "Common/ArithmeticProcessor/Lexer.h"
#include "Common/TypeDefs.h"

using namespace std;

class ArithmeticProcessor::ArithmeticExpression {
public:
	static ArithmeticExpression parse(ArithmeticProcessor::Lexer& lex);
	ArithmeticExpression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<int> constants);
	bool equals(const ArithmeticExpression& other);
	bool contains(const ArithmeticExpression& other);

	unordered_set<int> getConstants();
	unordered_set<VarRef> getVariables();

private:
	shared_ptr<ExpressionNode> root;
	unordered_set<VarRef> variables;
	unordered_set<int> constants;

	static shared_ptr<ExpressionNode> construct(Lexer& lex, unordered_set<VarRef>& variables, unordered_set<int>& constants, shared_ptr<ExpressionNode> lhs, int precedence);
	static shared_ptr<ExpressionNode> parseTerminal(Lexer& lex, unordered_set<VarRef>& variables, unordered_set<int>& constants);
	static int getPrecedence(ArithmeticOperator op);
};

#endif  // SPA_ARITHMETICEXPRESSION_H

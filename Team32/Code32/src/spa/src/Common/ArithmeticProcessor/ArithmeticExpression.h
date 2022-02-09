#ifndef SPA_ARITHMETICEXPRESSION_H
#define SPA_ARITHMETICEXPRESSION_H

#include <memory>
#include <unordered_set>

#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "Common/ArithmeticProcessor/ExpressionNode.h"
#include "Common/ArithmeticProcessor/LexerInterface.h"
#include "Common/TypeDefs.h"

using namespace std;

class Common::ArithmeticProcessor::ArithmeticExpression {
public:
	static ArithmeticExpression parse(LexerInterface& lex);
	ArithmeticExpression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<int> constants);
	bool equals(const ArithmeticExpression& other);
	bool contains(const ArithmeticExpression& other);

	unordered_set<int> getConstants();
	unordered_set<VarRef> getVariables();

private:
	shared_ptr<ExpressionNode> root;
	unordered_set<VarRef> variables;
	unordered_set<int> constants;

	static shared_ptr<ExpressionNode> construct(LexerInterface& lex, unordered_set<VarRef>& variables, unordered_set<int>& constants,
	                                            shared_ptr<ExpressionNode> lhs, int precedence);
	static shared_ptr<ExpressionNode> parseTerminal(LexerInterface& lex, unordered_set<VarRef>& variables, unordered_set<int>& constants);
	static int getPrecedence(ArithmeticOperator op);
};

#endif  // SPA_ARITHMETICEXPRESSION_H

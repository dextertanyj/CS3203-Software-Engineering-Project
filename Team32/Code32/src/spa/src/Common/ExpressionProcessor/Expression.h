#ifndef SPA_EXPRESSION_H
#define SPA_EXPRESSION_H

#include <memory>
#include <string>
#include <unordered_set>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LexerInterface.h"
#include "Common/TypeDefs.h"

using namespace std;

class Common::ExpressionProcessor::Expression {
public:
	static Expression parse(LexerInterface& lex, bool (*acceptor)(string op));
	Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<ConstVal> constants);
	bool equals(const Expression& other);
	bool contains(const Expression& other);

	unordered_set<ConstVal> getConstants();
	unordered_set<VarRef> getVariables();

private:
	shared_ptr<ExpressionNode> root;
	unordered_set<VarRef> variables;
	unordered_set<ConstVal> constants;

	static shared_ptr<ExpressionNode> construct(LexerInterface& lex, bool (*acceptor)(string op), unordered_set<VarRef>& variables,
	                                            unordered_set<ConstVal>& constants, shared_ptr<ExpressionNode> lhs, int precedence);
	static shared_ptr<ExpressionNode> parseTerminal(LexerInterface& lex, bool (*acceptor)(string op), unordered_set<VarRef>& variables,
	                                                unordered_set<ConstVal>& constants);
	static shared_ptr<ExpressionNode> parseTerminalSafe(LexerInterface& lex, bool (*acceptor)(string op), unordered_set<VarRef>& variables,
	                                                    unordered_set<ConstVal>& constants);
	static int getPrecedence(MathematicalOperator op);
};

#endif  // SPA_EXPRESSION_H

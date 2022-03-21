#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H

#include <memory>
#include <string>
#include <unordered_set>
#include <variant>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LexerInterface.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/TypeDefs.h"

using namespace std;

class Common::ExpressionProcessor::Expression {
public:
	static Expression parse(LexerInterface& lex, ExpressionType type);
	Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<ConstVal> constants);
	bool equals(const Expression& other);
	bool contains(const Expression& other);

	unordered_set<ConstVal> getConstants();
	unordered_set<VarRef> getVariables();

private:
	shared_ptr<ExpressionNode> root;
	string traversal;
	unordered_set<VarRef> variables;
	unordered_set<ConstVal> constants;

	static ParenthesizedExpression construct(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
	                                            unordered_set<ConstVal>& constants, ParenthesizedExpression lhs, int precedence);
	static ParenthesizedExpression parseTerminal(LexerInterface& lex, Acceptor acceptor,
	                                                                             unordered_set<VarRef>& variables,
	                                                                             unordered_set<ConstVal>& constants);
	static shared_ptr<ExpressionNode> parseTerminalSafe(LexerInterface& lex, Acceptor acceptor, unordered_set<VarRef>& variables,
	                                                    unordered_set<ConstVal>& constants);
	static shared_ptr<ExpressionNode> getExpression(const ParenthesizedExpression& expression);
	static bool checkExpressionType(const shared_ptr<ExpressionNode>& expression, ExpressionType type);
	static int getPrecedence(MathematicalOperator opr);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H

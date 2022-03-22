#ifndef SPA_EXPRESSIONPARSER_H
#define SPA_EXPRESSIONPARSER_H

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LexerInterface.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::ExpressionParser {
public:
	ExpressionParser() = default;
	Expression parse(LexerInterface& lex, ExpressionType type);

private:
	unordered_set<VarRef> variables;
	unordered_set<ConstVal> constants;

	ParenthesizedExpression construct(LexerInterface& lex, Acceptor acceptor, ParenthesizedExpression lhs, int precedence);
	ParenthesizedExpression parseTerminal(LexerInterface& lex, Acceptor acceptor);
	shared_ptr<ExpressionNode> parseTerminalSafe(LexerInterface& lex, Acceptor acceptor);
	shared_ptr<UnaryLogicalNode> parseUnaryLogical(LexerInterface& lex, Acceptor acceptor);

	static shared_ptr<ExpressionNode> getExpression(const ParenthesizedExpression& expression);
	static bool checkExpressionType(const shared_ptr<ExpressionNode>& expression, ExpressionType type);
	static int getPrecedence(MathematicalOperator opr);
};

#endif  // SPA_EXPRESSIONPARSER_H

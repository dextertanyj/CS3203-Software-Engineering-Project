#ifndef SPA_EXPRESSIONPARSER_H
#define SPA_EXPRESSIONPARSER_H

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LexerInterface.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::ExpressionParser {
public:
	ExpressionParser(LexerInterface& lex, ExpressionType type);
	Expression parse();

private:
	LexerInterface& lex;
	ExpressionType type;
	unordered_set<VarRef> variables;
	unordered_set<ConstVal> constants;

	ParenthesizedExpression construct(Acceptor acceptor, ParenthesizedExpression lhs, int precedence);
	ParenthesizedExpression parseTerminal(Acceptor acceptor);
	shared_ptr<ExpressionNode> parseTerminalSafe(Acceptor acceptor);
	shared_ptr<UnaryLogicalNode> parseUnaryLogical();
	shared_ptr<BinaryLogicalNode> parseBinaryLogical(ParenthesizedExpression lhs);

	static shared_ptr<ExpressionNode> getExpression(const ParenthesizedExpression& expression);
	static bool checkExpressionType(const shared_ptr<ExpressionNode>& expression, ExpressionType type);
	static int getPrecedence(MathematicalOperator opr);
};

#endif  // SPA_EXPRESSIONPARSER_H

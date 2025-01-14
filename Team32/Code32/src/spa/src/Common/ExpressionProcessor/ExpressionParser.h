#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPARSER_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPARSER_H

#include <memory>

#include "Common/ExpressionProcessor/BinaryLogicalNode.h"
#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LexerInterface.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/ExpressionProcessor/UnaryLogicalNode.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::ExpressionParser {
public:
	ExpressionParser(LexerInterface& lex, ExpressionType type);
	Expression parse();

private:
	LexerInterface& lex;
	ExpressionType type;
	VarRefSet variables;
	ConstValSet constants;

	ParenthesizedExpression construct(Acceptor acceptor, ParenthesizedExpression lhs, int precedence);
	ParenthesizedExpression parseTerminal(Acceptor acceptor);
	std::shared_ptr<ExpressionNode> parseTerminalSafe(Acceptor acceptor);
	std::shared_ptr<UnaryLogicalNode> parseUnaryLogical();
	std::shared_ptr<BinaryLogicalNode> parseBinaryLogical(const ParenthesizedExpression& lhs);

	static std::shared_ptr<ExpressionNode> getExpression(const ParenthesizedExpression& expression);
	static bool checkExpressionType(const std::shared_ptr<ExpressionNode>& expression, ExpressionType type);
	static int getPrecedence(MathematicalOperator opr);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPARSER_H

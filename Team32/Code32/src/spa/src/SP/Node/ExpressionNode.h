#ifndef SPA_SRC_SP_NODE_EXPRESSIONNODE_H
#define SPA_SRC_SP_NODE_EXPRESSIONNODE_H

#include <memory>

#include "Common/ExpressionProcessor/Expression.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

class SP::Node::ExpressionNode {
public:
	explicit ExpressionNode(Common::ExpressionProcessor::Expression expression);
	[[nodiscard]] Common::ExpressionProcessor::Expression extract() const;
	[[nodiscard]] bool equals(const std::shared_ptr<ExpressionNode>& object) const;

	static std::unique_ptr<ExpressionNode> parseExpression(Lexer& lex, Common::ExpressionProcessor::ExpressionType type);

private:
	Common::ExpressionProcessor::Expression expression;
};

#endif  // SPA_SRC_SP_NODE_EXPRESSIONNODE_H

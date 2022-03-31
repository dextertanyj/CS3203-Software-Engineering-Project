#ifndef SPA_SRC_SP_NODE_EXPRESSIONNODE_H
#define SPA_SRC_SP_NODE_EXPRESSIONNODE_H

#include <memory>

#include "Common/ExpressionProcessor/Expression.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

class SP::Node::ExpressionNode {
public:
	explicit ExpressionNode(Common::ExpressionProcessor::Expression expression);
	Common::ExpressionProcessor::Expression extract() const;
	[[nodiscard]] bool equals(const shared_ptr<ExpressionNode>& object) const;

	static unique_ptr<ExpressionNode> parseExpression(Lexer& lex, Common::ExpressionProcessor::ExpressionType type);

private:
	Common::ExpressionProcessor::Expression expression;
};

#endif  // SPA_SRC_SP_NODE_EXPRESSIONNODE_H

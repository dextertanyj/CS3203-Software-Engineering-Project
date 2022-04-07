#ifndef SPA_SRC_SP_NODE_EXPRESSIONNODE_H
#define SPA_SRC_SP_NODE_EXPRESSIONNODE_H

#include <memory>

#include "Common/ExpressionProcessor/Expression.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

class SP::Node::ExpressionNode {
public:
	explicit ExpressionNode(Common::EP::Expression expression);
	[[nodiscard]] Common::EP::Expression extract() const;
	[[nodiscard]] bool equals(const std::shared_ptr<ExpressionNode>& object) const;

	static std::unique_ptr<ExpressionNode> parse(Lexer& lex, Common::EP::ExpressionType type);

private:
	Common::EP::Expression expression;
};

#endif  // SPA_SRC_SP_NODE_EXPRESSIONNODE_H

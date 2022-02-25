#ifndef SPA_SRC_SP_NODE_EXPRESSIONNODE_H
#define SPA_SRC_SP_NODE_EXPRESSIONNODE_H

#include <memory>

#include "Common/ExpressionProcessor/Expression.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

using namespace std;

class SP::Node::ExpressionNode {
public:
	static unique_ptr<ExpressionNode> parseExpression(Lexer& lex, Common::ExpressionProcessor::ExpressionType type);
	explicit ExpressionNode(Common::ExpressionProcessor::Expression expression);
	Common::ExpressionProcessor::Expression extract();
	bool equals(const shared_ptr<ExpressionNode>& object);

private:
	Common::ExpressionProcessor::Expression expression;
};

#endif  // SPA_SRC_SP_NODE_EXPRESSIONNODE_H

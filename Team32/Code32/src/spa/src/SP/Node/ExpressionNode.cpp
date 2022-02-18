#include "SP/Node/ExpressionNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

SP::Node::ExpressionNode::ExpressionNode(Common::ExpressionProcessor::Expression expression) : expression(move(expression)) {}

unique_ptr<SP::Node::ExpressionNode> SP::Node::ExpressionNode::parseExpression(Lexer& lex,
                                                                               Common::ExpressionProcessor::ExpressionType type) {
	Common::ExpressionProcessor::Expression expression = Common::ExpressionProcessor::Expression::parse(lex, type);
	return make_unique<ExpressionNode>(expression);
}

Common::ExpressionProcessor::Expression SP::Node::ExpressionNode::extract() { return expression; }

bool SP::Node::ExpressionNode::equals(const shared_ptr<ExpressionNode>& object) { return this->expression.equals(object->expression); }

#include "SP/Node/ArithmeticExpressionNode.h"

using namespace std;

SP::Node::ArithmeticExpressionNode::ArithmeticExpressionNode(Common::ArithmeticProcessor::ArithmeticExpression expression)
	: expression(move(expression)) {}

unique_ptr<SP::Node::ArithmeticExpressionNode> SP::Node::ArithmeticExpressionNode::parseArithmeticExpression(Lexer& lex) {
	Common::ArithmeticProcessor::ArithmeticExpression expression = Common::ArithmeticProcessor::ArithmeticExpression::parse(lex);
	return make_unique<ArithmeticExpressionNode>(expression);
}

Common::ArithmeticProcessor::ArithmeticExpression SP::Node::ArithmeticExpressionNode::extract() { return expression; }

bool SP::Node::ArithmeticExpressionNode::equals(shared_ptr<ArithmeticExpressionNode> object) {
	return this->expression.equals(object->expression);
}

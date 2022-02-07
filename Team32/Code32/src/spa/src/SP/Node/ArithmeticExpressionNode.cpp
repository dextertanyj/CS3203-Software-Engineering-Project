#include "SP/Node/ArithmeticExpressionNode.h"

#include <memory>

using namespace std;
using namespace SP;

ArithmeticExpressionNode::ArithmeticExpressionNode(Common::ArithmeticProcessor::ArithmeticExpression expression) : expression(move(expression)) {}

unique_ptr<ArithmeticExpressionNode> ArithmeticExpressionNode::parseArithmeticExpression(Lexer& lex) {
	Common::ArithmeticProcessor::ArithmeticExpression expression = Common::ArithmeticProcessor::ArithmeticExpression::parse(lex);
	return make_unique<ArithmeticExpressionNode>(expression);
}

Common::ArithmeticProcessor::ArithmeticExpression ArithmeticExpressionNode::extract() {
	return expression;
}

bool ArithmeticExpressionNode::equals(shared_ptr<ArithmeticExpressionNode> object) {
    return this->expression.equals(object->expression);
}

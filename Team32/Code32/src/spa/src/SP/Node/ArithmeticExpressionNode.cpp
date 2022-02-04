#include "SP/Node/ArithmeticExpressionNode.h"

#include <memory>

using namespace std;
using namespace SP;

ArithmeticExpressionNode::ArithmeticExpressionNode(ArithmeticProcessor::ArithmeticExpression expression) : expression(move(expression)) {}

unique_ptr<ArithmeticExpressionNode> ArithmeticExpressionNode::parseArithmeticExpression(Lexer& lex) {
	ArithmeticProcessor::ArithmeticExpression expression = ArithmeticProcessor::ArithmeticExpression::parse(lex);
	return make_unique<ArithmeticExpressionNode>(expression);
}

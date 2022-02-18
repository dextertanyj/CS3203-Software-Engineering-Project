#include "Common/ExpressionProcessor/ParenthesesWrapper.h"

#include <cassert>

using namespace std;

Common::ExpressionProcessor::ParenthesesWrapper::ParenthesesWrapper(shared_ptr<ExpressionNode> expression)
	: expression(move(expression)) {}

shared_ptr<Common::ExpressionProcessor::ExpressionNode> Common::ExpressionProcessor::ParenthesesWrapper::getExpression() {
	return expression;
}

bool Common::ExpressionProcessor::ParenthesesWrapper::equals(shared_ptr<ExpressionNode> /*object*/) {
	throw ExpressionProcessorException("Logic error");
}

bool Common::ExpressionProcessor::ParenthesesWrapper::contains(shared_ptr<ExpressionNode> /*object*/) {
	throw ExpressionProcessorException("Logic error");
}

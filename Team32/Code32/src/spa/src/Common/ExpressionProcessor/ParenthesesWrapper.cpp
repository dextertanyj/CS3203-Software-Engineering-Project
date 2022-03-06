#include "Common/ExpressionProcessor/ParenthesesWrapper.h"

#include <stdexcept>

using namespace std;

Common::ExpressionProcessor::ParenthesesWrapper::ParenthesesWrapper(shared_ptr<ExpressionNode> expression) : expression(move(expression)) {}

shared_ptr<Common::ExpressionProcessor::ExpressionNode> Common::ExpressionProcessor::ParenthesesWrapper::getExpression() {
	return expression;
}

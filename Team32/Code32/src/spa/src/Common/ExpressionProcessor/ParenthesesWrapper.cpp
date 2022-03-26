#include "Common/ExpressionProcessor/ParenthesesWrapper.h"

#include <stdexcept>

using namespace std;

Common::ExpressionProcessor::ParenthesesWrapper::ParenthesesWrapper(ParenthesizedExpression expression) {
	visit(Visitor{[&](const ParenthesesWrapper& wrapper) {
					  if (dynamic_pointer_cast<LogicalNode>(wrapper.expression) != nullptr) {
						  throw ExpressionProcessorException("Unsupported nested parentheses detected.");
					  }
					  this->expression = wrapper.expression;
				  },
	              [&](const shared_ptr<ExpressionNode>& node) { this->expression = node; }},
	      expression);
}

shared_ptr<Common::ExpressionProcessor::ExpressionNode> Common::ExpressionProcessor::ParenthesesWrapper::getExpression() const {
	return expression;
}

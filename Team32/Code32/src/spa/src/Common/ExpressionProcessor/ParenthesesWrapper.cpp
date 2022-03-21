#include "Common/ExpressionProcessor/ParenthesesWrapper.h"

#include <stdexcept>

#include "Common/ExpressionProcessor/LogicalNode.h"

using namespace std;

Common::ExpressionProcessor::ParenthesesWrapper::ParenthesesWrapper(ParenthesizedExpression expression) {
	visit(Visitor{[&](const ParenthesesWrapper& wrapper) {
					  if (dynamic_pointer_cast<LogicalNode>(wrapper.expression) != nullptr) {
						  throw ExpressionProcessorException("Unsupported nested parentheses in expression.");
					  }
					  this->expression = wrapper.expression;
				  },
	              [&](const shared_ptr<ExpressionNode>& node) { this->expression = node; }},
	      expression);
}

shared_ptr<Common::ExpressionProcessor::ExpressionNode> Common::ExpressionProcessor::ParenthesesWrapper::getExpression() const {
	return expression;
}

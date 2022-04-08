#include "Common/ExpressionProcessor/ParenthesesWrapper.h"

#include <variant>

#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/TypeDefs.h"

using namespace std;

/**
 * This constructor checks for nested parentheses and rejects any already parenthesized logical expression.
 * For all other expression types, any existing parentheses is unwrapped to ensure only a single wrapper applies to any expression.
 */
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

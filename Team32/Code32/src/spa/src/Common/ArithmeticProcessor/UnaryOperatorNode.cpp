#include "UnaryOperatorNode.h"

#include <memory>

Common::ArithmeticProcessor::UnaryOperatorNode::UnaryOperatorNode(MathematicalOperator op, shared_ptr<ExpressionNode> expression)
	: op(op), expression(std::move(expression)) {}

bool Common::ArithmeticProcessor::UnaryOperatorNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<UnaryOperatorNode> other = dynamic_pointer_cast<UnaryOperatorNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op &&
	       other->expression->equals(this->expression)
}

bool Common::ArithmeticProcessor::UnaryOperatorNode::contains(shared_ptr<ExpressionNode> other) {
	return equals(other) || expression->contains(other)
}
#include "UnaryLogicalNode.h"

#include <memory>

Common::ExpressionProcessor::UnaryLogicalNode::UnaryLogicalNode(MathematicalOperator op, shared_ptr<LogicalNode> expression)
	: op(op), expression(std::move(expression)) {}

bool Common::ExpressionProcessor::UnaryLogicalNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<UnaryLogicalNode> other = dynamic_pointer_cast<UnaryLogicalNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op && other->expression->equals(this->expression);
}

bool Common::ExpressionProcessor::UnaryLogicalNode::contains(shared_ptr<ExpressionNode> other) {
	return equals(other) || expression->contains(other);
}

#include "BinaryOperatorNode.h"

#include <memory>

Common::ArithmeticProcessor::BinaryOperatorNode::BinaryOperatorNode(MathematicalOperator op, shared_ptr<ExpressionNode> lhs, shared_ptr<ExpressionNode> rhs)
	: op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

bool Common::ArithmeticProcessor::BinaryOperatorNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<BinaryOperatorNode> other = dynamic_pointer_cast<BinaryOperatorNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op &&
	       other->lhs->equals(this->lhs) &&
	       other->rhs->equals(this->rhs);
}

bool Common::ArithmeticProcessor::BinaryOperatorNode::contains(shared_ptr<ExpressionNode> other) {
	return equals(other) || this->lhs->contains(other) || this->rhs->contains(other);
}
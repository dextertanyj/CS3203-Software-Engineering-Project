#include "ArithmeticNode.h"

#include <memory>

Common::ExpressionProcessor::ArithmeticNode::ArithmeticNode(MathematicalOperator op, shared_ptr<AtomicNode> lhs, shared_ptr<AtomicNode> rhs)
	: op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

bool Common::ExpressionProcessor::ArithmeticNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<ArithmeticNode> other = dynamic_pointer_cast<ArithmeticNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op &&
	       other->lhs->equals(this->lhs) &&
	       other->rhs->equals(this->rhs);
}

bool Common::ExpressionProcessor::ArithmeticNode::contains(shared_ptr<ExpressionNode> other) {
	return equals(other) || this->lhs->contains(other) || this->rhs->contains(other);
}
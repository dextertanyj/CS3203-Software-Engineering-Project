#include "RelationalNode.h"

Common::ExpressionProcessor::RelationalNode::RelationalNode(MathematicalOperator op, shared_ptr<AtomicNode> lhs, shared_ptr<AtomicNode> rhs)
	: op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

bool Common::ExpressionProcessor::RelationalNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<RelationalNode> other = dynamic_pointer_cast<RelationalNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op &&
	       other->lhs->equals(this->lhs) &&
	       other->rhs->equals(this->rhs);
}

bool Common::ExpressionProcessor::RelationalNode::contains(shared_ptr<ExpressionNode> other) {
	return equals(other) || this->lhs->contains(other) || this->rhs->contains(other);
}

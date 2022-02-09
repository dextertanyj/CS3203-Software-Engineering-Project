#include "BinaryLogicalNode.h"

Common::ExpressionProcessor::BinaryLogicalNode::BinaryLogicalNode(MathematicalOperator op, shared_ptr<LogicalNode> lhs,
                                                                  shared_ptr<LogicalNode> rhs)
	: op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

bool Common::ExpressionProcessor::BinaryLogicalNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<BinaryLogicalNode> other = dynamic_pointer_cast<BinaryLogicalNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op && other->lhs->equals(this->lhs) && other->rhs->equals(this->rhs);
}

bool Common::ExpressionProcessor::BinaryLogicalNode::contains(shared_ptr<ExpressionNode> other) {
	return equals(other) || this->lhs->contains(other) || this->rhs->contains(other);
}
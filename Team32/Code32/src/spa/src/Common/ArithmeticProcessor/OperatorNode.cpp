//
// Created by Dexter on 3/2/22.
//

#include "OperatorNode.h"

#include <utility>

ArithmeticProcessor::OperatorNode::OperatorNode(ArithmeticOperator op, shared_ptr<ExpressionNode> lhs, shared_ptr<ExpressionNode> rhs)
	: op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

bool ArithmeticProcessor::OperatorNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<OperatorNode> other = dynamic_pointer_cast<OperatorNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op &&
	       other->lhs->equals(this->lhs) &&
	       other->rhs->equals(this->rhs);
}

bool ArithmeticProcessor::OperatorNode::contains(shared_ptr<ExpressionNode> other) {
	return equals(other) || this->lhs->contains(other) || this->rhs->contains(other);
}
#include "Common/ExpressionProcessor/BinaryLogicalNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Common::ExpressionProcessor::BinaryLogicalNode::BinaryLogicalNode(MathematicalOperator op, shared_ptr<LogicalNode> lhs,
                                                                  shared_ptr<LogicalNode> rhs)
	: lhs(move(lhs)), rhs(move(rhs)) {
	if (!OperatorAcceptor::acceptBinaryLogical(op)) {
		throw ExpressionProcessorException("Expected binary logical operator.");
	}
	this->op = op;
}

bool Common::ExpressionProcessor::BinaryLogicalNode::equals(shared_ptr<ExpressionNode> object) {
	shared_ptr<BinaryLogicalNode> other = dynamic_pointer_cast<BinaryLogicalNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op && other->lhs->equals(this->lhs) && other->rhs->equals(this->rhs);
}

bool Common::ExpressionProcessor::BinaryLogicalNode::contains(shared_ptr<ExpressionNode> object) {
	return equals(object) || this->lhs->contains(object) || this->rhs->contains(object);
}
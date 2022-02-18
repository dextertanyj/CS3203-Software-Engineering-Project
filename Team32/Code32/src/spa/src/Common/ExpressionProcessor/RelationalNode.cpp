#include "Common/ExpressionProcessor/RelationalNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Common::ExpressionProcessor::RelationalNode::RelationalNode(MathematicalOperator op, shared_ptr<AtomicNode> lhs, shared_ptr<AtomicNode> rhs)
	: lhs(move(lhs)), rhs(move(rhs)) {
	if (!OperatorAcceptor::acceptRelationalStrict(op)) {
		throw ExpressionProcessorException("Expected relational operator");
	}
	this->op = op;
}

bool Common::ExpressionProcessor::RelationalNode::equals(shared_ptr<ExpressionNode> object) {
	shared_ptr<RelationalNode> other = dynamic_pointer_cast<RelationalNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op && other->lhs->equals(this->lhs) && other->rhs->equals(this->rhs);
}

bool Common::ExpressionProcessor::RelationalNode::contains(shared_ptr<ExpressionNode> object) {
	return equals(object) || this->lhs->contains(object) || this->rhs->contains(object);
}

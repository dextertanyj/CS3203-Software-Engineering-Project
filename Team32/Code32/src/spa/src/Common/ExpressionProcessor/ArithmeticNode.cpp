#include "Common/ExpressionProcessor/ArithmeticNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Common::ExpressionProcessor::ArithmeticNode::ArithmeticNode(MathematicalOperator op, shared_ptr<AtomicNode> lhs, shared_ptr<AtomicNode> rhs)
	: lhs(move(lhs)), rhs(move(rhs)) {
	if (!OperatorAcceptor::acceptArithmetic(op)) {
		throw ExpressionProcessorException("Expected arithmetic operator.");
	}
	this->op = op;
}

bool Common::ExpressionProcessor::ArithmeticNode::equals(shared_ptr<ExpressionNode> object) {
	shared_ptr<ArithmeticNode> other = dynamic_pointer_cast<ArithmeticNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->op == this->op && other->lhs->equals(this->lhs) && other->rhs->equals(this->rhs);
}

bool Common::ExpressionProcessor::ArithmeticNode::contains(shared_ptr<ExpressionNode> object) {
	return equals(object) || this->lhs->contains(object) || this->rhs->contains(object);
}
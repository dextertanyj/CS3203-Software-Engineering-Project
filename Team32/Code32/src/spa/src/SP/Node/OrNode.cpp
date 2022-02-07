#include "SP/Node/OrNode.h"

SP::Node::OrNode::OrNode(unique_ptr<ConditionalExpressionNode> lhs, unique_ptr<ConditionalExpressionNode> rhs)
	: lhs(move(lhs)), rhs(move(rhs)) {}

SP::UsageInfo SP::Node::OrNode::extract() {
	UsageInfo usage;
	UsageInfo lhs_usage = lhs->extract();
	UsageInfo rhs_usage = rhs->extract();
	usage.variables.insert(lhs_usage.variables.begin(), lhs_usage.variables.end());
	usage.variables.insert(rhs_usage.variables.begin(), rhs_usage.variables.end());
	usage.constants.insert(lhs_usage.constants.begin(), lhs_usage.constants.end());
	usage.constants.insert(rhs_usage.constants.begin(), rhs_usage.constants.end());
	return usage;
}

bool SP::Node::OrNode::equals(shared_ptr<ConditionalExpressionNode> object) {
	shared_ptr<OrNode> other = dynamic_pointer_cast<OrNode>(object);
	if (other == nullptr) {
		return false;
	}
	return this->lhs->equals(move(other->lhs)) && this->rhs->equals(move(other->rhs));
}

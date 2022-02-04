#include "SP/Node/AndNode.h"

AndNode::AndNode(unique_ptr<ConditionalExpressionNode> lhs, unique_ptr<ConditionalExpressionNode> rhs) : lhs(move(lhs)), rhs(move(rhs)) {}

UsageInfo AndNode::extract() {
	UsageInfo usage;
	UsageInfo lhs_usage = lhs->extract();
	UsageInfo rhs_usage = rhs->extract();
	usage.variables.insert(end(usage.variables), lhs_usage.variables.begin(), lhs_usage.variables.end());
	usage.variables.insert(end(usage.variables), rhs_usage.variables.begin(), rhs_usage.variables.end());
	usage.constants.insert(end(usage.constants), lhs_usage.constants.begin(), lhs_usage.constants.end());
	usage.constants.insert(end(usage.constants), rhs_usage.constants.begin(), rhs_usage.constants.end());
	return usage;
}

bool AndNode::equals(shared_ptr<ConditionalExpressionNode> object) {
    shared_ptr<AndNode> other = dynamic_pointer_cast<AndNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->lhs->equals(move(other->lhs)) && this->rhs->equals(move(other->rhs));
}

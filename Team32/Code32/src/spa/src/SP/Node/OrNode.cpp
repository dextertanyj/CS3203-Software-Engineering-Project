#include "SP/Node/OrNode.h"

OrNode::OrNode(unique_ptr<ConditionalExpressionNode> lhs, unique_ptr<ConditionalExpressionNode> rhs) : lhs(move(lhs)), rhs(move(rhs)) {}

bool OrNode::equals(shared_ptr<ConditionalExpressionNode> object) {
    shared_ptr<OrNode> other = dynamic_pointer_cast<OrNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->lhs->equals(move(other->lhs)) && this->rhs->equals(move(other->rhs));
}

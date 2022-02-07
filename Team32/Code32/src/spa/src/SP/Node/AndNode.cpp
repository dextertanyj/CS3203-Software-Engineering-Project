#include "SP/Node/AndNode.h"

AndNode::AndNode(unique_ptr<ConditionalExpressionNode> lhs, unique_ptr<ConditionalExpressionNode> rhs) : lhs(move(lhs)), rhs(move(rhs)) {}

bool AndNode::equals(shared_ptr<ConditionalExpressionNode> object) {
    shared_ptr<AndNode> other = dynamic_pointer_cast<AndNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->lhs->equals(move(other->lhs)) && this->rhs->equals(move(other->rhs));
}

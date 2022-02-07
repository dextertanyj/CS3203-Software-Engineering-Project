#include "SP/Node/NotNode.h"

SP::Node::NotNode::NotNode(unique_ptr<ConditionalExpressionNode> expression) : expression(move(expression)) {}

SP::UsageInfo SP::Node::NotNode::extract() { return expression->extract(); }

bool SP::Node::NotNode::equals(shared_ptr<ConditionalExpressionNode> object) {
    shared_ptr<NotNode> other = dynamic_pointer_cast<NotNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->expression->equals(move(other->expression));
}

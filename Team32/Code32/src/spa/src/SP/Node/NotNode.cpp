#include "SP/Node/NotNode.h"

NotNode::NotNode(unique_ptr<ConditionalExpressionNode> expression) : expression(move(expression)) {}

bool NotNode::equals(shared_ptr<ConditionalExpressionNode> object) {
    shared_ptr<NotNode> other = dynamic_pointer_cast<NotNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->expression->equals(move(other->expression));
}

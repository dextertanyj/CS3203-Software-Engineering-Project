#include "OrNode.h"

OrNode::OrNode(unique_ptr<ConditionalExpressionNode> lhs, unique_ptr<ConditionalExpressionNode> rhs):
        lhs(move(lhs)),
        rhs(move(rhs)){
}

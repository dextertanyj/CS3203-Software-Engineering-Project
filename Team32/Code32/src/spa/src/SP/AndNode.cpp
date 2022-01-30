#include "SP/AndNode.h"

AndNode::AndNode(unique_ptr<ConditionalExpressionNode> lhs, unique_ptr<ConditionalExpressionNode> rhs):
        lhs(move(lhs)),
        rhs(move(rhs)) {
}

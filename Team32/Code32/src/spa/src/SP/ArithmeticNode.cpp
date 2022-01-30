#include "SP/ArithmeticNode.h"

ArithmeticNode::ArithmeticNode(ArithmeticOperator op, unique_ptr<ArithmeticExpressionNode> lhs,
                               unique_ptr<ArithmeticExpressionNode> rhs):
                               op(op),
                               lhs(move(lhs)),
                               rhs(move(lhs)){
}

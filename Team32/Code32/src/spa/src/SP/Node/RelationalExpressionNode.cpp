#include "RelationalExpressionNode.h"

RelationalExpressionNode::RelationalExpressionNode(RelationalOperator op, unique_ptr<RelationalFactorNode> lhs,
                                                   unique_ptr<RelationalFactorNode> rhs):
                                                   op(op),
                                                   lhs(move(lhs)),
                                                   rhs(move(rhs)) {
}

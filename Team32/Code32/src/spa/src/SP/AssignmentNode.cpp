#include "AssignmentNode.h"

AssignmentNode::AssignmentNode(StmtRef stmtNo, VarNode varName, ArithmeticExpressionNode expr):
        StatementNode(stmtNo),
        varName(varName),
        expr(expr){
}

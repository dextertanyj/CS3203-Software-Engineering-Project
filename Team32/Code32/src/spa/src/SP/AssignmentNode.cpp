#include "AssignmentNode.h"

using namespace std;

AssignmentNode::AssignmentNode(StmtRef stmtNo, unique_ptr<VarNode> varName, unique_ptr<ArithmeticExpressionNode> expr):
        StatementNode(stmtNo),
        varName(move(varName)),
        expr(move(expr)){
}

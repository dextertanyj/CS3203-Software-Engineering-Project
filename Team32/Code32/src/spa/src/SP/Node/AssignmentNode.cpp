#include "SP/Node/AssignmentNode.h"

using namespace std;

AssignmentNode::AssignmentNode(StmtRef stmtNo, unique_ptr<VariableNode> assignee, unique_ptr<ArithmeticExpressionNode> expression):
        StatementNode(stmtNo),
        assignee(move(assignee)),
        expression(move(expression)){
}

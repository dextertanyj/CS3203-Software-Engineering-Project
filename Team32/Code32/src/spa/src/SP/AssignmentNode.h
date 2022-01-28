#ifndef SPA_ASSIGNMENTNODE_H
#define SPA_ASSIGNMENTNODE_H

#include <string>

#include "SP/ArithmeticExpressionNode.h"
#include "SP/StatementNode.h"
#include "SP/VariableNode.h"

using namespace std;

class AssignmentNode : public StatementNode {
public:
    AssignmentNode(StmtRef stmtNo, unique_ptr<VariableNode> assignee, unique_ptr<ArithmeticExpressionNode> expression);
private:
    unique_ptr<VariableNode> assignee;
    unique_ptr<ArithmeticExpressionNode> expression;
};

#endif //SPA_ASSIGNMENTNODE_H

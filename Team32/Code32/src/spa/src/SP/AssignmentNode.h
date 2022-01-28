#ifndef SPA_ASSIGNMENTNODE_H
#define SPA_ASSIGNMENTNODE_H

#include "StatementNode.h"
#include <string>

using namespace std;

typedef string VarNode;
typedef string ArithmeticExpressionNode;

class AssignmentNode : public StatementNode {
public:
    AssignmentNode(StmtRef stmtNo, unique_ptr<VarNode> varName, unique_ptr<ArithmeticExpressionNode> expr);
private:
    unique_ptr<VarNode> varName;
    unique_ptr<ArithmeticExpressionNode> expr;
};

#endif //SPA_ASSIGNMENTNODE_H

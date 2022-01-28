#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include <string>

#include "SP/ConditionalExpressionNode.h"
#include "SP/StatementListNode.h"
#include "SP/StatementNode.h"

using namespace std;

class IfNode : public StatementNode {
public:
    IfNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> ifStmtLst, unique_ptr<StatementListNode> elseStmtLst);
private:
    unique_ptr<ConditionalExpressionNode> condExpr;
    unique_ptr<StatementListNode> ifStmtLst;
    unique_ptr<StatementListNode> elseStmtLst;
};

#endif //SPA_IFNODE_H

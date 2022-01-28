#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include "StatementNode.h"
#include "StatementListNode.h"
#include <string>

using namespace std;

typedef string CondExpr;

class IfNode : public StatementNode {
public:
    IfNode(StmtRef stmtNo, unique_ptr<CondExpr> condExpr, unique_ptr<StatementListNode> ifStmtLst, unique_ptr<StatementListNode> elseStmtLst);
private:
    unique_ptr<CondExpr> condExpr;
    unique_ptr<StatementListNode> ifStmtLst;
    unique_ptr<StatementListNode> elseStmtLst;
};

#endif //SPA_IFNODE_H

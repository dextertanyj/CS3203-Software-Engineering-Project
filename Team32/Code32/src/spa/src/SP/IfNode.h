#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include "StatementNode.h"
#include "StatementListNode.h"
#include <string>

using namespace std;

typedef string CondExpr;

class IfNode : public StatementNode {
public:
    IfNode(StmtRef stmtNo, CondExpr condExpr, StatementListNode ifStmtLst, StatementListNode elseStmtLst);
private:
    CondExpr condExpr;
    StatementListNode ifStmtLst;
    StatementListNode elseStmtLst;
};

#endif //SPA_IFNODE_H

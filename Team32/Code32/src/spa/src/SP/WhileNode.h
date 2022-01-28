#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include "StatementNode.h"
#include "StatementListNode.h"
#include <string>

using namespace std;

typedef string CondExpr;

class WhileNode : public StatementNode {
public:
    WhileNode(StmtRef stmtNo, CondExpr condExpr, StatementListNode stmtLst);
private:
    CondExpr condExpr;
    StatementListNode stmtLst;
};

#endif //SPA_WHILENODE_H

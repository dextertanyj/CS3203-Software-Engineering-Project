#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include "StatementNode.h"
#include "StatementListNode.h"
#include <string>

using namespace std;

typedef string CondExpr;

class WhileNode : public StatementNode {
public:
    WhileNode(StmtRef stmtNo, unique_ptr<CondExpr> condExpr, unique_ptr<StatementListNode> stmtLst);
private:
    unique_ptr<CondExpr> condExpr;
    unique_ptr<StatementListNode> stmtLst;
};

#endif //SPA_WHILENODE_H

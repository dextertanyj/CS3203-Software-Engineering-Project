#ifndef SPA_STATEMENTLISTNODE_H
#define SPA_STATEMENTLISTNODE_H

#include "StatementNode.h"

using namespace std;

class StatementListNode {
public:
    StatementListNode();
    void addStatementNode(StatementNode stmtNode);
private:
    vector<unique_ptr<StatementNode>> stmtList;
};

#endif //SPA_STATEMENTLISTNODE_H

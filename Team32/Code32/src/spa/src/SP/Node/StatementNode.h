#ifndef SPA_STATEMENTNODE_H
#define SPA_STATEMENTNODE_H

#include "Common/TypeDefs.h"

class StatementNode {
public:
    StatementNode(StmtRef stmtNo);
private:
    StmtRef stmtNo;
};

#endif //SPA_STATEMENTNODE_H

#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include "Common/TypeDefs.h"
#include "StatementNode.h"

class CallNode : public StatementNode {
public:
    CallNode(StmtRef stmtNo, ProcRef procedure);
private:
    ProcRef procedure;
};

#endif //SPA_CALLNODE_H

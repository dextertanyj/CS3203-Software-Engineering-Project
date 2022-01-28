#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include "SP/ProcedureNode.h"
#include "SP/StatementNode.h"

class CallNode : public StatementNode {
public:
    CallNode(StmtRef stmtNo, unique_ptr<ProcedureNode> procedure);
private:
    unique_ptr<ProcedureNode> procedure;
};

#endif //SPA_CALLNODE_H

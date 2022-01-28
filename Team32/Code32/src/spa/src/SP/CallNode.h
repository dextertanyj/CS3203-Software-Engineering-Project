#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include "StatementNode.h"
#include "ProcedureNode.h"

class CallNode : public StatementNode {
public:
    CallNode(StmtRef stmtNo, unique_ptr<ProcedureNode> procNode);
private:
    unique_ptr<ProcedureNode> procNode;
};

#endif //SPA_CALLNODE_H

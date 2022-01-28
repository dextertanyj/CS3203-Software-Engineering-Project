#include "CallNode.h"

CallNode::CallNode(StmtRef stmtNo, ProcedureNode& procNode):
        StatementNode(stmtNo),
        procNode(procNode) {
}

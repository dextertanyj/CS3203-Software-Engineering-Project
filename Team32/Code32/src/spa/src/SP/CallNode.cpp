#include "CallNode.h"

using namespace std;

CallNode::CallNode(StmtRef stmtNo, unique_ptr<ProcedureNode> procNode):
        StatementNode(stmtNo),
        procNode(move(procNode)) {
}

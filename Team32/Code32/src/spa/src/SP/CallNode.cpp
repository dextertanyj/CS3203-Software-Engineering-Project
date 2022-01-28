#include "SP/CallNode.h"

using namespace std;

CallNode::CallNode(StmtRef stmtNo, unique_ptr<ProcedureNode> procedure):
        StatementNode(stmtNo),
        procedure(move(procedure)) {
}

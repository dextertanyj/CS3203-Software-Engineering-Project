#include "SP/Node/CallNode.h"

using namespace std;

CallNode::CallNode(StmtRef stmtNo, ProcRef procedure):
        StatementNode(stmtNo),
        procedure(move(procedure)) {
}

#include "SP/CallNode.h"

using namespace std;

CallNode::CallNode(StmtRef stmtNo, ProcRef procedure):
        StatementNode(stmtNo),
        procedure(move(procedure)) {
}

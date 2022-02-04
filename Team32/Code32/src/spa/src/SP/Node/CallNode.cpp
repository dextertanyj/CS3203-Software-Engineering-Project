#include "SP/Node/CallNode.h"

CallNode::CallNode(StmtRef stmtNo, ProcRef procedure) : StatementNode(stmtNo), procedure(move(procedure)) {}

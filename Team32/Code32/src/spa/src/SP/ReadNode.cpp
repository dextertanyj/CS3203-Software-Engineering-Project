#include "ReadNode.h"

ReadNode::ReadNode(StmtRef stmtNo, unique_ptr<VarNode> varName):
        StatementNode(stmtNo),
        varName(move(varName)) {
}

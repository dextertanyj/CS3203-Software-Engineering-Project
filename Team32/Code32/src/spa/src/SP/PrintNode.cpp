#include "PrintNode.h"

PrintNode::PrintNode(StmtRef stmtNo, VarNode varName):
        StatementNode(stmtNo),
        varName(varName) {
}

#include "PrintNode.h"

using namespace std;

PrintNode::PrintNode(StmtRef stmtNo, unique_ptr<VarNode> varName):
        StatementNode(stmtNo),
        varName(move(varName)) {
}

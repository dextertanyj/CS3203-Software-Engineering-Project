#include "SP/Node/ReadNode.h"

ReadNode::ReadNode(StmtRef stmtNo, unique_ptr<VariableNode> variable):
        StatementNode(stmtNo),
        variable(move(variable)) {
}

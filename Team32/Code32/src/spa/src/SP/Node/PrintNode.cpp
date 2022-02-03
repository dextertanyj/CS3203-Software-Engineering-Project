#include "SP/Node/PrintNode.h"

using namespace std;

PrintNode::PrintNode(StmtRef stmtNo, unique_ptr<VariableNode> variable) : StatementNode(stmtNo), variable(move(variable)) {}

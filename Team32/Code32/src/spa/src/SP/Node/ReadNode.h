#ifndef SPA_READNODE_H
#define SPA_READNODE_H

#include <string>

#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

using namespace std;

class ReadNode : public StatementNode {
public:
    ReadNode(StmtRef stmtNo, unique_ptr<VariableNode> variable);
private:
    unique_ptr<VariableNode> variable;
};

#endif //SPA_READNODE_H

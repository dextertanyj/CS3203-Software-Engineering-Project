#ifndef SPA_READNODE_H
#define SPA_READNODE_H

#include <string>

#include "StatementNode.h"
#include "VariableNode.h"

using namespace std;

class ReadNode : public StatementNode {
public:
    ReadNode(StmtRef stmtNo, unique_ptr<VariableNode> variable);
private:
    unique_ptr<VariableNode> variable;
};

#endif //SPA_READNODE_H

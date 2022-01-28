#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H

#include <string>

#include "SP/StatementNode.h"
#include "SP/VariableNode.h"

using namespace std;

class PrintNode : public StatementNode {
public:
    PrintNode(StmtRef stmtNo, unique_ptr<VariableNode> variable);
private:
    unique_ptr<VariableNode> variable;
};

#endif //SPA_PRINTNODE_H

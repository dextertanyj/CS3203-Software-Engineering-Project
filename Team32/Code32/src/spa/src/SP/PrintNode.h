#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H

#include "StatementNode.h"
#include <string>

using namespace std;

typedef string VarNode;

class PrintNode : public StatementNode {
public:
    PrintNode(StmtRef stmtNo, VarNode varName);
private:
    VarNode varName;
};

#endif //SPA_PRINTNODE_H

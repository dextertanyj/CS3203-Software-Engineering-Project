#ifndef SPA_READNODE_H
#define SPA_READNODE_H

#include "StatementNode.h"
#include <string>

using namespace std;

typedef string VarNode;

class ReadNode : public StatementNode {
public:
    ReadNode(StmtRef stmtNo, VarNode varName);
private:
    VarNode varName;
};

#endif //SPA_READNODE_H

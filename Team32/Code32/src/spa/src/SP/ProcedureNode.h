#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

#include "StatementListNode.h"
#include <string>

using namespace std;

class ProcedureNode {
public:
    ProcedureNode(string name, unique_ptr<StatementListNode> stmtLst);
private:
    string name;
    unique_ptr<StatementListNode> stmtLst;
};

#endif //SPA_PROCEDURENODE_H

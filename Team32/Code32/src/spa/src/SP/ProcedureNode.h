#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

#include "StatementListNode.h"
#include <string>

using namespace std;

class ProcedureNode {
public:
    ProcedureNode(string name, StatementListNode stmtLst);
private:
    string name;
    StatementListNode stmtLst;
};


#endif //SPA_PROCEDURENODE_H

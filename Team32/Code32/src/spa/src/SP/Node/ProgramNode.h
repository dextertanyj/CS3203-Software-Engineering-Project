#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

#include "ProcedureNode.h"

using namespace std;

class ProgramNode {
public:
    ProgramNode();
    void addProcedureNode(unique_ptr<ProcedureNode> procedure);
private:
    vector<shared_ptr<ProcedureNode>> procedures;
};

#endif //SPA_PROGRAMNODE_H

#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

#include "ProcedureNode.h"

using namespace std;

class ProgramNode {
public:
    ProgramNode();
    void addProcedureNode(ProcedureNode procNode);
private:
    vector<unique_ptr<ProcedureNode>> procedures;
};

#endif //SPA_PROGRAMNODE_H

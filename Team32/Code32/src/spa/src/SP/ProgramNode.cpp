#include "ProgramNode.h"
#include <vector>

using namespace std;

ProgramNode::ProgramNode() = default;

void ProgramNode::addProcedureNode(unique_ptr<ProcedureNode>  procNode) {
    procedures.push_back(move(procNode));
}

#include <vector>

#include "ProgramNode.h"

using namespace std;

ProgramNode::ProgramNode() = default;

void ProgramNode::addProcedureNode(unique_ptr<ProcedureNode> procedure) {
    procedures.push_back(move(procedure));
}

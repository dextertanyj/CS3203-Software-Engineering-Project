#include "ProgramNode.h"
#include <vector>

using namespace std;

ProgramNode::ProgramNode() = default;

void ProgramNode::addProcedureNode(ProcedureNode procNode) {
    procedures.push_back(make_unique<ProcedureNode>(procNode));
}

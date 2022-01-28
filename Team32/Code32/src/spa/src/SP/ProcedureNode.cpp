#include "ProcedureNode.h"

#include <utility>

using namespace std;

ProcedureNode::ProcedureNode(string name, unique_ptr<StatementListNode> stmtLst):
        name(move(name)),
        stmtLst(move(stmtLst)) {
}

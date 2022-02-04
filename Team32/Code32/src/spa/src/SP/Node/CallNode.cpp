#include "SP/Node/CallNode.h"

CallNode::CallNode(StmtRef stmtNo, ProcRef procedure) : StatementNode(stmtNo), procedure(move(procedure)) {}

bool CallNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<CallNode> other = dynamic_pointer_cast<CallNode>(object);
    if (other == nullptr) {
        return false;
    }
    return other->stmtNo == this->stmtNo && other->procedure == this->procedure;
}

#include "SP/Node/ReadNode.h"

ReadNode::ReadNode(StmtRef stmtNo, unique_ptr<VariableNode> variable) : StatementNode(stmtNo), variable(move(variable)) {}

bool ReadNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<ReadNode> other = dynamic_pointer_cast<ReadNode>(object);
    if (other == nullptr) {
        return false;
    }
    return other->stmtNo == this->stmtNo && other->variable->equals(move(this->variable));
}

#include "SP/Node/PrintNode.h"

using namespace std;

PrintNode::PrintNode(StmtRef stmtNo, unique_ptr<VariableNode> variable) : StatementNode(stmtNo), variable(move(variable)) {}

bool PrintNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<PrintNode> other = dynamic_pointer_cast<PrintNode>(object);
    if (other == nullptr) {
        return false;
    }
    return other->stmtNo == this->stmtNo && other->variable->equals(move(this->variable));
}

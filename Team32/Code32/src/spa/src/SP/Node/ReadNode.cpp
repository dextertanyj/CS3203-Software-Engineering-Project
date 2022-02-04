#include "SP/Node/ReadNode.h"

ReadNode::ReadNode(StmtRef stmtNo, unique_ptr<VariableNode> variable) : StatementNode(stmtNo), variable(move(variable)) {}

StmtInfo ReadNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Read);
	return {stmt_ref, StmtType::Read};
}

bool ReadNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<ReadNode> other = dynamic_pointer_cast<ReadNode>(object);
    if (other == nullptr) {
        return false;
    }
    return other->stmtNo == this->stmtNo && other->variable->equals(move(this->variable));
}

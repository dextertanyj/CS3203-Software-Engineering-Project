#include "SP/Node/CallNode.h"

CallNode::CallNode(StmtRef stmtNo, ProcRef procedure) : StatementNode(stmtNo), procedure(move(procedure)) {}

StmtInfo CallNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Call);
	return {stmt_ref, StmtType::Call};
}

bool CallNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<CallNode> other = dynamic_pointer_cast<CallNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->getStmtRef() == other->getStmtRef() && this->procedure == other->procedure;
}

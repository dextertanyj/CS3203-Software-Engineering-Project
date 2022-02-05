#include "SP/Node/PrintNode.h"

using namespace std;

PrintNode::PrintNode(StmtRef stmtNo, unique_ptr<VariableNode> variable) : StatementNode(stmtNo), variable(move(variable)) {}
StmtInfo PrintNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setUses(stmt_ref, variable->extract());
	pkb.setStmtType(stmt_ref, StmtType::Print);
	return {stmt_ref, StmtType::Print};
}

bool PrintNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<PrintNode> other = dynamic_pointer_cast<PrintNode>(object);
    if (other == nullptr) {
        return false;
    }
    return other->getStmtRef() == this->getStmtRef() && other->variable->equals(move(this->variable));
}

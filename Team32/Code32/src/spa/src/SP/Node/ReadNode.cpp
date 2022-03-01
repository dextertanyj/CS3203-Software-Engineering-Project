#include "SP/Node/ReadNode.h"

SP::Node::ReadNode::ReadNode(StmtRef stmt_no, unique_ptr<VariableNode> variable) : StatementNode(stmt_no), variable(move(variable)) {}

StmtRef SP::Node::ReadNode::extract(PKB::StorageUpdateInterface& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Read);
	pkb.setModifies(stmt_ref, variable->extract());
	return stmt_ref;
}

bool SP::Node::ReadNode::equals(const shared_ptr<StatementNode>& object) {
	shared_ptr<ReadNode> other = dynamic_pointer_cast<ReadNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->getStmtRef() == this->getStmtRef() && other->variable->equals(this->variable);
}

unique_ptr<SP::Node::ReadNode> SP::Node::ReadNode::parseReadStatement(Lexer& lex, StmtRef& statement_count) {
	unique_ptr<VariableNode> variable = VariableNode::parseVariable(lex);
	lex.nextIf(";");
	return make_unique<ReadNode>(statement_count++, move(variable));
}

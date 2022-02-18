#include "SP/Node/PrintNode.h"

using namespace std;

SP::Node::PrintNode::PrintNode(StmtRef stmt_no, unique_ptr<VariableNode> variable) : StatementNode(stmt_no), variable(move(variable)) {}

StmtRef SP::Node::PrintNode::extract(PKB &pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Print);
	pkb.setUses(stmt_ref, variable->extract());
	return stmt_ref;
}

bool SP::Node::PrintNode::equals(const shared_ptr<StatementNode> &object) {
	shared_ptr<PrintNode> other = dynamic_pointer_cast<PrintNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->getStmtRef() == this->getStmtRef() && other->variable->equals(this->variable);
}

unique_ptr<SP::Node::PrintNode> SP::Node::PrintNode::parsePrintStatement(Lexer &lex, StmtRef &statement_count) {
	unique_ptr<VariableNode> variable = VariableNode::parseVariable(lex);
	lex.nextIf(";");
	return make_unique<PrintNode>(statement_count++, move(variable));
}

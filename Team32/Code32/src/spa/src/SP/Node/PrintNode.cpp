#include "SP/Node/PrintNode.h"

using namespace std;

SP::Node::PrintNode::PrintNode(StmtRef stmtNo, unique_ptr<VariableNode> variable) : StatementNode(stmtNo), variable(move(variable)) {}

StmtInfo SP::Node::PrintNode::extract(PKB &pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setUses(stmt_ref, variable->extract());
	pkb.setStmtType(stmt_ref, StmtType::Print);
	return {stmt_ref, StmtType::Print};
}

bool SP::Node::PrintNode::equals(shared_ptr<StatementNode> object) {
	shared_ptr<PrintNode> other = dynamic_pointer_cast<PrintNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->getStmtRef() == this->getStmtRef() && other->variable->equals(move(this->variable));
}

unique_ptr<SP::Node::PrintNode> SP::Node::PrintNode::parsePrintStatement(Lexer &lex, int &statement_count) {
	unique_ptr<VariableNode> variable = VariableNode::parseVariable(lex);
	lex.nextIf(";");
	return make_unique<PrintNode>(statement_count++, move(variable));
}

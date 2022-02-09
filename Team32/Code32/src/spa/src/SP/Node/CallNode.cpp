#include "SP/Node/CallNode.h"

#include "Common/Validator.h"

SP::Node::CallNode::CallNode(StmtRef stmtNo, ProcRef procedure) : StatementNode(stmtNo), procedure(move(procedure)) {}

StmtInfo SP::Node::CallNode::extract(PKB &pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Call);
	return {stmt_ref, StmtType::Call};
}

bool SP::Node::CallNode::equals(shared_ptr<StatementNode> object) {
	shared_ptr<CallNode> other = dynamic_pointer_cast<CallNode>(object);
	if (other == nullptr) {
		return false;
	}
	return this->getStmtRef() == other->getStmtRef() && this->procedure == other->procedure;
}

unique_ptr<SP::Node::CallNode> SP::Node::CallNode::parseCallStatement(Lexer &lex, int &statement_count) {
	ProcRef name = lex.readToken();
	if (!Common::Validator::validateName(name)) {
		throw SP::ParseException("Invalid procedure name");
	}
	lex.nextIf(";");
	return make_unique<CallNode>(statement_count++, name);
}

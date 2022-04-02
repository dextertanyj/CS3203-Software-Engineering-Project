#include "SP/Node/CallNode.h"

#include <utility>

#include "Common/Validator.h"

using namespace std;

SP::Node::CallNode::CallNode(StmtRef stmt_no, ProcRef procedure) : StatementNode(stmt_no), procedure(move(procedure)) {}

StmtRef SP::Node::CallNode::extract(PKB::StorageUpdateInterface& pkb) const {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Call);
	pkb.setCall(stmt_ref, procedure);
	return stmt_ref;
}

bool SP::Node::CallNode::equals(const shared_ptr<StatementNode>& object) const {
	shared_ptr<CallNode> other = dynamic_pointer_cast<CallNode>(object);
	if (other == nullptr) {
		return false;
	}
	return this->getStmtRef() == other->getStmtRef() && this->procedure == other->procedure;
}

unique_ptr<SP::Node::CallNode> SP::Node::CallNode::parseCallStatement(Lexer& lex, StmtRef& statement_count) {
	ProcRef name = lex.readToken();
	if (!Common::Validator::validateName(name)) {
		throw ParseException("Invalid procedure name.");
	}
	lex.nextIf(";");
	return make_unique<CallNode>(statement_count++, name);
}

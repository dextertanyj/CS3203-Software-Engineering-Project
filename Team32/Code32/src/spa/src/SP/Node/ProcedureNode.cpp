#include "SP/Node/ProcedureNode.h"

#include <utility>

#include "Common/Validator.h"
#include "SP/SP.h"

#define PROCEDURE "procedure"

using namespace std;

SP::Node::ProcedureNode::ProcedureNode(ProcRef name, unique_ptr<StatementListNode> statements, StmtRef start, StmtRef end)
	: name(move(name)), start(start), end(end), statements(move(statements)) {}

void SP::Node::ProcedureNode::extract(PKB::StorageUpdateInterface& pkb) const {
	statements->extract(pkb);
	pkb.setProc(name, start, end);
}

bool SP::Node::ProcedureNode::equals(const shared_ptr<ProcedureNode>& object) const {
	return this->name == object->name && this->statements->equals(object->statements) && this->start == object->start &&
	       this->end == object->end;
}

unique_ptr<SP::Node::ProcedureNode> SP::Node::ProcedureNode::parse(Lexer& lex, StmtRef& statement_count) {
	lex.nextIf(PROCEDURE);
	ProcRef name = lex.readToken();
	if (!Common::Validator::validateName(name)) {
		throw SP::ParseException("Invalid procedure name.");
	}
	lex.nextIf(OPEN_BRACES);
	StmtRef start = statement_count;
	unique_ptr<StatementListNode> statement_list = StatementListNode::parse(lex, statement_count);
	StmtRef end = statement_count - 1;
	lex.nextIf(CLOSE_BRACES);
	return make_unique<ProcedureNode>(name, move(statement_list), start, end);
}

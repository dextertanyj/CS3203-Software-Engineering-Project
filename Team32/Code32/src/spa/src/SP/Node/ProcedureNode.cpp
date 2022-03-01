#include "SP/Node/ProcedureNode.h"

#include "Common/Validator.h"

using namespace std;

SP::Node::ProcedureNode::ProcedureNode(string name, unique_ptr<StatementListNode> stmt_list, StmtRef start, StmtRef end)
	: name(move(name)), start(start), end(end), stmt_list(move(stmt_list)) {}

unique_ptr<SP::Node::ProcedureNode> SP::Node::ProcedureNode::parseProcedure(Lexer& lex, StmtRef& statement_count) {
	lex.nextIf("procedure");
	ProcRef name = lex.readToken();
	if (!Common::Validator::validateName(name)) {
		throw SP::ParseException("Invalid procedure name.");
	}
	lex.nextIf("{");
	StmtRef start = statement_count;
	unique_ptr<StatementListNode> statement_list = StatementListNode::parseStatementList(lex, statement_count);
	StmtRef end = statement_count - 1;
	lex.nextIf("}");
	return make_unique<ProcedureNode>(name, std::move(statement_list), start, end);
}

void SP::Node::ProcedureNode::extract(PKB::StorageUpdateInterface& pkb) { stmt_list->extract(pkb); }

bool SP::Node::ProcedureNode::equals(const shared_ptr<ProcedureNode>& object) {
	return this->name == object->name && this->stmt_list->equals(object->stmt_list) && this->start == object->start &&
	       this->end == object->end;
}

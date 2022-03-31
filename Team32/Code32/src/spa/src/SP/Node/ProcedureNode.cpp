#include "SP/Node/ProcedureNode.h"

#include "Common/Validator.h"

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

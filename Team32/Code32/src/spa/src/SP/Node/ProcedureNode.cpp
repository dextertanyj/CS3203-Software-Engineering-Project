#include "SP/Node/ProcedureNode.h"

#include "Common/Validator.h"

using namespace std;

SP::Node::ProcedureNode::ProcedureNode(string name, unique_ptr<StatementListNode> stmtLst, StmtRef start, StmtRef end)
    : name(move(name)), stmtLst(move(stmtLst)), start(start), end(end) {}

unique_ptr<SP::Node::ProcedureNode> SP::Node::ProcedureNode::parseProcedure(Lexer& lex, int& statement_count) {
	lex.nextIf("procedure");
	ProcRef name = lex.readToken();
	if (!Common::Validator::validateName(name)) {
		throw SP::ParseException("Invalid procedure name");
	}
	lex.nextIf("{");
    StmtRef start = statement_count;
	unique_ptr<StatementListNode> statement_list = StatementListNode::parseStatementList(lex, statement_count);
    StmtRef end = statement_count - 1;
	lex.nextIf("}");
	return make_unique<ProcedureNode>(name, std::move(statement_list), start, end);
}

bool SP::Node::ProcedureNode::extract(PKB& pkb) {
	stmtLst->extract(pkb);
	return true;
}

bool SP::Node::ProcedureNode::equals(const shared_ptr<ProcedureNode>& object) {
	return this->name == object->name && this->stmtLst->equals(object->stmtLst) && this->start == object->start
        && this->end == object->end;
}

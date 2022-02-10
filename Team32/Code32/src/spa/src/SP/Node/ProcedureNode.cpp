#include "SP/Node/ProcedureNode.h"

#include "Common/Validator.h"

using namespace std;

SP::Node::ProcedureNode::ProcedureNode(string name, unique_ptr<StatementListNode> stmtLst) : name(move(name)), stmtLst(move(stmtLst)) {}

unique_ptr<SP::Node::ProcedureNode> SP::Node::ProcedureNode::parseProcedure(Lexer& lex, int& statement_count) {
	lex.nextIf("procedure");
	ProcRef name = lex.readToken();
	if (!Common::Validator::validateName(name)) {
		throw SP::ParseException("Invalid procedure name");
	}
	lex.nextIf("{");
	unique_ptr<StatementListNode> statement_list = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	return make_unique<ProcedureNode>(name, std::move(statement_list));
}

bool SP::Node::ProcedureNode::extract(PKB& pkb) {
	StmtInfoList children = stmtLst->extract(pkb);
	return true;
}

bool SP::Node::ProcedureNode::equals(shared_ptr<ProcedureNode> object) {
	return this->name == object->name && this->stmtLst->equals(object->stmtLst);
}

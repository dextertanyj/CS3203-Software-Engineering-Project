#include "SP/Node/ProcedureNode.h"

#include <memory>

#include "Common/Validator.h"
#include "SP/SP.h"

using namespace std;
using namespace SP;

ProcedureNode::ProcedureNode(string name, unique_ptr<StatementListNode> stmtLst) : name(move(name)), stmtLst(move(stmtLst)) {}

unique_ptr<ProcedureNode> ProcedureNode::parseProcedure(Lexer& lex, int& statement_count) {
	lex.nextIf("procedure");
	ProcRef name = lex.readToken();
	if (!Validator::validateName(name)) {
		throw SP::ParseException("Invalid procedure name");
	}
	lex.nextIf("{");
	unique_ptr<StatementListNode> statement_list = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	return make_unique<ProcedureNode>(name, std::move(statement_list));
}

bool ProcedureNode::extract(PKB& pkb) {
	StmtInfoList children = stmtLst->extract(pkb);
	return true;
}

bool ProcedureNode::equals(shared_ptr<ProcedureNode> object) {
	return this->name == object->name && this->stmtLst->equals(move(object->stmtLst));
}

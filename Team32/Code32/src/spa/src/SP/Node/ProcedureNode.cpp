#include "SP/Node/ProcedureNode.h"

#include <memory>

#include "SP/ParseException.h"
#include "SP/Validator.h"

using namespace std;

ProcedureNode::ProcedureNode(string name, unique_ptr<StatementListNode> stmtLst) : name(move(name)), stmtLst(move(stmtLst)) {}

unique_ptr<ProcedureNode> ProcedureNode::parseProcedure(Lexer& lex, int& statement_count) {
	lex.next_if("procedure");
	ProcRef name = lex.read_token();
	if (!Validator::validateName(name)) {
		throw ParseException("Invalid procedure name");
	}
	lex.next_if("{");
	unique_ptr<StatementListNode> statement_list = StatementListNode::parseStatementList(lex, statement_count);
	lex.next_if("}");
	return make_unique<ProcedureNode>(name, std::move(statement_list));
}
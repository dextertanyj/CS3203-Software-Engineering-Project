#include "SP/Node/StatementListNode.h"

#include <vector>

using namespace std;

StatementListNode::StatementListNode() = default;

void StatementListNode::addStatementNode(unique_ptr<StatementNode> statement) { stmtList.push_back(move(statement)); }

unique_ptr<StatementListNode> StatementListNode::parseStatementList(Lexer& lex, int& statement_count) {
	unique_ptr<StatementListNode> statement_list = make_unique<StatementListNode>();
	do {
		statement_list->addStatementNode(StatementNode::parseStatement(lex, statement_count));
	} while (lex.peek_token() != "}");
	return statement_list;
}
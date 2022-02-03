#ifndef SPA_STATEMENTLISTNODE_H
#define SPA_STATEMENTLISTNODE_H

#include "SP/Node/StatementNode.h"

using namespace std;

class StatementListNode {
public:
	StatementListNode();
	void addStatementNode(unique_ptr<StatementNode> statement);
	static unique_ptr<StatementListNode> parseStatementList(Lexer& lex, int& statement_count);

private:
	vector<shared_ptr<StatementNode>> stmtList;
};

#endif  // SPA_STATEMENTLISTNODE_H

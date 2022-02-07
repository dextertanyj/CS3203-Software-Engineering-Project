#ifndef SPA_STATEMENTLISTNODE_H
#define SPA_STATEMENTLISTNODE_H

#include "PKB/PKB.h"
#include "SP/Node/StatementNode.h"

using namespace std;

class StatementListNode {
public:
	StatementListNode();
	void addStatementNode(unique_ptr<StatementNode> statement);
	static unique_ptr<StatementListNode> parseStatementList(Lexer& lex, int& statement_count);
	StmtInfoList extract(PKB& pkb);
    virtual bool equals(shared_ptr<StatementListNode> object);
    vector<shared_ptr<StatementNode>> getStatementList();

private:
	vector<shared_ptr<StatementNode>> stmtList;
};

#endif  // SPA_STATEMENTLISTNODE_H

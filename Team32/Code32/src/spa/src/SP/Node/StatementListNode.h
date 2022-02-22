#ifndef SPA_SRC_SP_NODE_STATEMENTLISTNODE_H
#define SPA_SRC_SP_NODE_STATEMENTLISTNODE_H

#include <memory>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"

using namespace std;

class SP::Node::StatementListNode {
public:
	StatementListNode();
	void addStatementNode(unique_ptr<StatementNode> statement);
	static unique_ptr<StatementListNode> parseStatementList(Lexer& lex, StmtRef& statement_count);
	vector<StmtRef> extract(PKB::Storage& pkb);
	bool equals(const shared_ptr<StatementListNode>& object);
	vector<shared_ptr<StatementNode>> getStatementList();

private:
	vector<shared_ptr<StatementNode>> stmt_list;
};

#endif  // SPA_SRC_SP_NODE_STATEMENTLISTNODE_H

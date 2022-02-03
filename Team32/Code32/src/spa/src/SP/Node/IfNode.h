#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include <string>

#include "SP/Node/ConditionalExpressionNode.h"
#include "SP/Node/StatementListNode.h"
#include "SP/Node/StatementNode.h"

using namespace std;

class IfNode : public StatementNode {
public:
	IfNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> ifStmtLst,
	       unique_ptr<StatementListNode> elseStmtLst);
	static unique_ptr<IfNode> parseIfStatement(Lexer& lex, int& statement_count);

private:
	unique_ptr<ConditionalExpressionNode> condExpr;
	unique_ptr<StatementListNode> ifStmtLst;
	unique_ptr<StatementListNode> elseStmtLst;
};

#endif  // SPA_IFNODE_H

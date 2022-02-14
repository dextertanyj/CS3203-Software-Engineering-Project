#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include <memory>
#include <string>

#include "PKB/PKB.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementListNode.h"
#include "SP/Node/StatementNode.h"

using namespace std;

class SP::Node::IfNode : public StatementNode {
public:
	IfNode(StmtRef stmtNo, unique_ptr<ExpressionNode> condExpr, unique_ptr<StatementListNode> ifStmtLst,
	       unique_ptr<StatementListNode> elseStmtLst);
	static unique_ptr<IfNode> parseIfStatement(Lexer& lex, int& statement_count);
	StmtInfo extract(PKB& pkb) override;
	bool equals(shared_ptr<StatementNode> object) override;

private:
	shared_ptr<ExpressionNode> condExpr;
	shared_ptr<StatementListNode> ifStmtLst;
	shared_ptr<StatementListNode> elseStmtLst;
};

#endif  // SPA_IFNODE_H

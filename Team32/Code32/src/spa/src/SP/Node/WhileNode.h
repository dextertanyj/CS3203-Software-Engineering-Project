#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/StatementListNode.h"
#include "SP/Node/StatementNode.h"

using namespace std;

class SP::Node::WhileNode : public StatementNode {
public:
	WhileNode(StmtRef stmtNo, unique_ptr<ExpressionNode> condExpr, unique_ptr<StatementListNode> stmtLst);
	static unique_ptr<WhileNode> parseWhileStatement(Lexer& lex, StmtRef& statement_count);
	StmtRef extract(PKB& pkb) override;
	bool equals(const shared_ptr<StatementNode>& object) override;

private:
	shared_ptr<ExpressionNode> condExpr;
	shared_ptr<StatementListNode> stmtLst;
};

#endif  // SPA_WHILENODE_H

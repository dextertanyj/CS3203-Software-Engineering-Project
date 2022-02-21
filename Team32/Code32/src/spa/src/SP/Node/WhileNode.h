#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/StatementListNode.h"
#include "SP/Node/StatementNode.h"

using namespace std;

class SP::Node::WhileNode : public StatementNode {
public:
	WhileNode(StmtRef stmt_no, unique_ptr<ExpressionNode> cond_expr, unique_ptr<StatementListNode> stmt_list);
	static unique_ptr<WhileNode> parseWhileStatement(Lexer& lex, StmtRef& statement_count);
	StmtRef extract(PKB::Storage& pkb) override;
	bool equals(const shared_ptr<StatementNode>& object) override;

private:
	shared_ptr<ExpressionNode> cond_expr;
	shared_ptr<StatementListNode> stmt_list;
};

#endif  // SPA_WHILENODE_H

#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include <memory>
#include <string>

#include "PKB/PKB.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementListNode.h"
#include "SP/Node/StatementNode.h"
#include "Common/TypeDefs.h"

using namespace std;

class SP::Node::IfNode : public StatementNode {
public:
	IfNode(StmtRef stmt_no, unique_ptr<ExpressionNode> cond_expr, unique_ptr<StatementListNode> if_stmt_list,
	       unique_ptr<StatementListNode> else_stmt_list);
	static unique_ptr<IfNode> parseIfStatement(Lexer& lex, StmtRef& statement_count);
	StmtRef extract(PKB& pkb) override;
	bool equals(const shared_ptr<StatementNode>& object) override;

private:
	shared_ptr<ExpressionNode> cond_expr;
	shared_ptr<StatementListNode> if_stmt_list;
	shared_ptr<StatementListNode> else_stmt_list;
};

#endif  // SPA_IFNODE_H

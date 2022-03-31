#ifndef SPA_SRC_SP_NODE_WHILENODE_H
#define SPA_SRC_SP_NODE_WHILENODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/StatementListNode.h"
#include "SP/Node/StatementNode.h"

class SP::Node::WhileNode : public StatementNode {
public:
	WhileNode(StmtRef stmt_no, unique_ptr<ExpressionNode> cond_expr, unique_ptr<StatementListNode> stmt_list);
	StmtRef extract(PKB::StorageUpdateInterface& pkb) const override;
	[[nodiscard]] bool equals(const shared_ptr<StatementNode>& object) const override;

	static unique_ptr<WhileNode> parseWhileStatement(Lexer& lex, StmtRef& statement_count);

private:
	shared_ptr<ExpressionNode> cond_expr;
	shared_ptr<StatementListNode> stmt_list;
};

#endif  // SPA_SRC_SP_NODE_WHILENODE_H

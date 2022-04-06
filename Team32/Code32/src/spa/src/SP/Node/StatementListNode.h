#ifndef SPA_SRC_SP_NODE_STATEMENTLISTNODE_H
#define SPA_SRC_SP_NODE_STATEMENTLISTNODE_H

#include <memory>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/StorageUpdateInterface.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"

class SP::Node::StatementListNode {
public:
	StatementListNode() = default;
	void addStatementNode(std::unique_ptr<StatementNode> statement);
	std::vector<StmtRef> extract(PKB::StorageUpdateInterface& pkb) const;
	[[nodiscard]] bool equals(const std::shared_ptr<StatementListNode>& object) const;
	[[nodiscard]] std::vector<std::shared_ptr<StatementNode>> getStatementList() const;

	static std::unique_ptr<StatementListNode> parseStatementList(Lexer& lex, StmtRef& statement_count);

private:
	std::vector<std::shared_ptr<StatementNode>> stmt_list;
};

#endif  // SPA_SRC_SP_NODE_STATEMENTLISTNODE_H

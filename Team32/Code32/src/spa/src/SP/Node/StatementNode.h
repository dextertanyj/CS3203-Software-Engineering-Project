#ifndef SPA_SRC_SP_NODE_STATEMENTNODE_H
#define SPA_SRC_SP_NODE_STATEMENTNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/StorageUpdateInterface.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

class SP::Node::StatementNode {
public:
	explicit StatementNode(StmtRef stmt_no);
	virtual StmtRef extract(PKB::StorageUpdateInterface& pkb) const = 0;
	[[nodiscard]] virtual bool equals(const std::shared_ptr<StatementNode>& object) const = 0;
	virtual ~StatementNode() = default;

	static std::unique_ptr<StatementNode> parseStatement(Lexer& lex, StmtRef& statement_count);

protected:
	[[nodiscard]] StmtRef getStmtRef() const;

private:
	StmtRef stmt_no;
};

#endif  // SPA_SRC_SP_NODE_STATEMENTNODE_H

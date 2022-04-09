#ifndef SPA_SRC_SP_NODE_PRINTREADNODE_H
#define SPA_SRC_SP_NODE_PRINTREADNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/StorageUpdateInterface.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

template <StmtType T>
class SP::Node::PrintReadNode : public StatementNode {
public:
	PrintReadNode(StmtRef stmt_no, std::unique_ptr<VariableNode> variable);
	StmtRef extract(PKB::StorageUpdateInterface& pkb) const override;
	[[nodiscard]] bool equals(const std::shared_ptr<StatementNode>& object) const override;

	static std::unique_ptr<PrintReadNode> parse(Lexer& lex, StmtRef& statement_count);

private:
	std::shared_ptr<VariableNode> variable;
};

#endif  // SPA_SRC_SP_NODE_PRINTREADNODE_H

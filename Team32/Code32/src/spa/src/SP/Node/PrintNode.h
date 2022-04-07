#ifndef SPA_SRC_SP_NODE_PRINTNODE_H
#define SPA_SRC_SP_NODE_PRINTNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/StorageUpdateInterface.h"
#include "SP/Lexer.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

class SP::Node::PrintNode : public StatementNode {
public:
	PrintNode(StmtRef stmt_no, std::unique_ptr<VariableNode> variable);
	StmtRef extract(PKB::StorageUpdateInterface& pkb) const override;
	[[nodiscard]] bool equals(const std::shared_ptr<StatementNode>& object) const override;

	static std::unique_ptr<PrintNode> parsePrintStatement(Lexer& lex, StmtRef& statement_count);

private:
	std::shared_ptr<VariableNode> variable;
};

#endif  // SPA_SRC_SP_NODE_PRINTNODE_H

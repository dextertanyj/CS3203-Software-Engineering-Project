#ifndef SPA_SRC_SP_NODE_PRINTNODE_H
#define SPA_SRC_SP_NODE_PRINTNODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Lexer.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

class SP::Node::PrintNode : public StatementNode {
public:
	PrintNode(StmtRef stmt_no, unique_ptr<VariableNode> variable);
	StmtRef extract(PKB::StorageUpdateInterface& pkb) const override;
	[[nodiscard]] bool equals(const shared_ptr<StatementNode>& object) const override;
	static unique_ptr<PrintNode> parsePrintStatement(Lexer& lex, StmtRef& statement_count);

private:
	shared_ptr<VariableNode> variable;
};

#endif  // SPA_SRC_SP_NODE_PRINTNODE_H

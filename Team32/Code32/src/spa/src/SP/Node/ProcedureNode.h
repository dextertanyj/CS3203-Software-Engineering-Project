#ifndef SPA_SRC_SP_NODE_PROCEDURENODE_H
#define SPA_SRC_SP_NODE_PROCEDURENODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/StorageUpdateInterface.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementListNode.h"

class SP::Node::ProcedureNode {
public:
	ProcedureNode(ProcRef name, std::unique_ptr<StatementListNode> statements, StmtRef start, StmtRef end);
	void extract(PKB::StorageUpdateInterface& pkb) const;
	[[nodiscard]] bool equals(const std::shared_ptr<ProcedureNode>& object) const;

	static std::unique_ptr<ProcedureNode> parse(Lexer& lex, StmtRef& statement_count);

private:
	ProcRef name;
	StmtRef start;
	StmtRef end;
	std::shared_ptr<StatementListNode> statements;
};

#endif  // SPA_SRC_SP_NODE_PROCEDURENODE_H

#ifndef SPA_SRC_SP_NODE_PROGRAMNODE_H
#define SPA_SRC_SP_NODE_PROGRAMNODE_H

#include <memory>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/StorageUpdateInterface.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"
#include "SP/Node/ProcedureNode.h"

class SP::Node::ProgramNode {
public:
	ProgramNode() = default;
	void addProcedureNode(std::unique_ptr<ProcedureNode> procedure);
	void extract(PKB::StorageUpdateInterface& pkb) const;
	[[nodiscard]] bool equals(const std::shared_ptr<ProgramNode>& object) const;
	[[nodiscard]] std::vector<std::shared_ptr<ProcedureNode>> getProcedures() const;

	static std::unique_ptr<ProgramNode> parse(Lexer& lex, StmtRef& statement_count);

private:
	std::vector<std::shared_ptr<ProcedureNode>> procedures;
};

#endif  // SPA_SRC_SP_NODE_PROGRAMNODE_H

#ifndef SPA_SRC_SP_NODE_PROGRAMNODE_H
#define SPA_SRC_SP_NODE_PROGRAMNODE_H

#include <memory>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Lexer.h"
#include "SP/Node/ProcedureNode.h"

class SP::Node::ProgramNode {
public:
	ProgramNode() = default;
	void addProcedureNode(unique_ptr<ProcedureNode> procedure);
	void extract(PKB::StorageUpdateInterface& pkb) const;
	[[nodiscard]] bool equals(const shared_ptr<ProgramNode>& object) const;
	[[nodiscard]] vector<shared_ptr<ProcedureNode>> getProcedures() const;

	static unique_ptr<ProgramNode> parseProgram(Lexer& lex, StmtRef& statement_count);

private:
	vector<shared_ptr<ProcedureNode>> procedures;
};

#endif  // SPA_SRC_SP_NODE_PROGRAMNODE_H

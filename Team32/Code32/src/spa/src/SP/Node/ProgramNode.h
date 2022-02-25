#ifndef SPA_SRC_SP_NODE_PROGRAMNODE_H
#define SPA_SRC_SP_NODE_PROGRAMNODE_H

#include <memory>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Lexer.h"
#include "SP/Node/ProcedureNode.h"

using namespace std;

class SP::Node::ProgramNode {
public:
	ProgramNode();
	void addProcedureNode(unique_ptr<ProcedureNode> procedure);
	static unique_ptr<ProgramNode> parseProgram(Lexer& lex, StmtRef& statement_count);
	void extract(PKB::Storage& pkb);
	bool equals(const shared_ptr<ProgramNode>& object);
	vector<shared_ptr<ProcedureNode>> getProcedures();

private:
	vector<shared_ptr<ProcedureNode>> procedures;
};

#endif  // SPA_SRC_SP_NODE_PROGRAMNODE_H

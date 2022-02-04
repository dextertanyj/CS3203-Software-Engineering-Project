#include "SP/Node/ProgramNode.h"

#include <vector>

using namespace std;
using namespace SP;

ProgramNode::ProgramNode() = default;

void ProgramNode::addProcedureNode(unique_ptr<ProcedureNode> procedure) { procedures.push_back(move(procedure)); }

unique_ptr<ProgramNode> ProgramNode::parseProgram(Lexer& lex, int& statement_count) {
	unique_ptr<ProgramNode> program = make_unique<ProgramNode>();
	do {
		program->addProcedureNode(ProcedureNode::parseProcedure(lex, statement_count));
	} while (!lex.peekToken().empty());
	return program;
}

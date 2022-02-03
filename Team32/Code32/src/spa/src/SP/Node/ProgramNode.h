#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

#include "SP/Lexer.h"
#include "SP/Node/ProcedureNode.h"

using namespace std;

class ProgramNode {
public:
	ProgramNode();
	void addProcedureNode(unique_ptr<ProcedureNode> procedure);
	static unique_ptr<ProgramNode> parseProgram(Lexer& lex, int& statement_count);

private:
	vector<shared_ptr<ProcedureNode>> procedures;
};

#endif  // SPA_PROGRAMNODE_H

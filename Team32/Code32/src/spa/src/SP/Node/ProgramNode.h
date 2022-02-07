#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

#include "SP/Lexer.h"
#include "SP/Node/ProcedureNode.h"

using namespace std;
using namespace SP;

class ProgramNode {
public:
	ProgramNode();
	void addProcedureNode(unique_ptr<ProcedureNode> procedure);
	static unique_ptr<ProgramNode> parseProgram(Lexer& lex, int& statement_count);
    bool equals(shared_ptr<ProgramNode> object);
    vector<shared_ptr<ProcedureNode>> getProcedures();

private:
	vector<shared_ptr<ProcedureNode>> procedures;
};

#endif  // SPA_PROGRAMNODE_H

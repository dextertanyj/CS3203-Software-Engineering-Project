#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

#include <string>

#include "PKB/PKB.h"
#include "SP/Lexer.h"
#include "SP/Node/StatementListNode.h"

using namespace std;
using namespace SP;

class ProcedureNode {
public:
	ProcedureNode(string name, unique_ptr<StatementListNode> stmtLst);
	static unique_ptr<ProcedureNode> parseProcedure(Lexer& lex, int& statement_count);
	bool extract(PKB& pkb);
    bool equals(shared_ptr<ProcedureNode> object);

private:
	string name;
	unique_ptr<StatementListNode> stmtLst;
};

#endif  // SPA_PROCEDURENODE_H
#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "SP/Lexer.h"
#include "SP/Node/StatementListNode.h"

using namespace std;

class SP::Node::ProcedureNode {
public:
	ProcedureNode(string name, unique_ptr<StatementListNode> stmtLst, StmtRef start, StmtRef end);
	static unique_ptr<ProcedureNode> parseProcedure(Lexer& lex, int& statement_count);
	bool extract(PKB& pkb);
	bool equals(shared_ptr<ProcedureNode>& object);

private:
	ProcRef name;
    StmtRef start;
    StmtRef end;
	shared_ptr<StatementListNode> stmtLst;
};

#endif  // SPA_PROCEDURENODE_H

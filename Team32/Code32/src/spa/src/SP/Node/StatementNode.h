#ifndef SPA_STATEMENTNODE_H
#define SPA_STATEMENTNODE_H

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "SP/Lexer.h"

using namespace SP;

class StatementNode {
public:
	explicit StatementNode(StmtRef stmtNo);
	static unique_ptr<StatementNode> parseStatement(Lexer& lex, int& statement_count);
	virtual StmtInfo extract(PKB& pkb) = 0;
  virtual bool equals(shared_ptr<StatementNode> object) = 0;

protected:
  StmtRef getStmtRef();
    StmtRef stmtNo;
};

#endif  // SPA_STATEMENTNODE_H

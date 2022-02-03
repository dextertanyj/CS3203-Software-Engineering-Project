#ifndef SPA_STATEMENTNODE_H
#define SPA_STATEMENTNODE_H

#include "Common/TypeDefs.h"
#include "SP/Lexer.h"

class StatementNode {
public:
	explicit StatementNode(StmtRef stmtNo);
	static unique_ptr<StatementNode> parseStatement(Lexer& lex, int& statement_count);

private:
	StmtRef stmtNo;
};

#endif  // SPA_STATEMENTNODE_H

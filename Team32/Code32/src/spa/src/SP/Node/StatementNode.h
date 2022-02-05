#ifndef SPA_STATEMENTNODE_H
#define SPA_STATEMENTNODE_H

#include "Common/TypeDefs.h"
#include "SP/Lexer.h"

using namespace SP;

class StatementNode {
public:
	explicit StatementNode(StmtRef stmtNo);
	static unique_ptr<StatementNode> parseStatement(Lexer& lex, int& statement_count);
    virtual bool equals(shared_ptr<StatementNode> object) = 0;

protected:
    StmtRef stmtNo;
};

#endif  // SPA_STATEMENTNODE_H

#ifndef SPA_STATEMENTNODE_H
#define SPA_STATEMENTNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

class SP::Node::StatementNode {
public:
	explicit StatementNode(StmtRef stmt_no);
	static unique_ptr<StatementNode> parseStatement(Lexer& lex, StmtRef& statement_count);
	virtual StmtRef extract(PKB& pkb) = 0;
	virtual bool equals(const shared_ptr<StatementNode>& object) = 0;
	virtual ~StatementNode() = default;

protected:
	[[nodiscard]] StmtRef getStmtRef() const;

private:
	StmtRef stmt_no;
};

#endif  // SPA_STATEMENTNODE_H

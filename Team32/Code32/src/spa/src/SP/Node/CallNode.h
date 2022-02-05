#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include "Common/TypeDefs.h"
#include "SP/Node/StatementNode.h"

class CallNode : public StatementNode {
public:
	CallNode(StmtRef stmtNo, ProcRef procedure);
	StmtInfo extract(PKB& pkb) override;
    bool equals(shared_ptr<StatementNode> object) override;
    static unique_ptr<CallNode> parseCallStatement(Lexer& lex, int& statement_count);

private:
	ProcRef procedure;
};

#endif  // SPA_CALLNODE_H

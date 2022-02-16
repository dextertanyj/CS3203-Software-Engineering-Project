#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"

class SP::Node::CallNode : public StatementNode {
public:
	CallNode(StmtRef stmtNo, ProcRef procedure);
	StmtRef extract(PKB& pkb) override;
	bool equals(const shared_ptr<StatementNode>& object) override;
	static unique_ptr<CallNode> parseCallStatement(Lexer& lex, int& statement_count);

private:
	ProcRef procedure;
};

#endif  // SPA_CALLNODE_H

#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"

class SP::Node::CallNode : public StatementNode {
public:
	CallNode(StmtRef stmt_no, ProcRef procedure);
	StmtRef extract(PKB::Storage& pkb) override;
	bool equals(const shared_ptr<StatementNode>& object) override;
	static unique_ptr<CallNode> parseCallStatement(Lexer& lex, StmtRef& statement_count);

private:
	ProcRef procedure;
};

#endif  // SPA_CALLNODE_H

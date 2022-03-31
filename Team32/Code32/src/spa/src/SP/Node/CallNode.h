#ifndef SPA_SRC_SP_NODE_CALLNODE_H
#define SPA_SRC_SP_NODE_CALLNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"

class SP::Node::CallNode : public StatementNode {
public:
	CallNode(StmtRef stmt_no, ProcRef procedure);
	StmtRef extract(PKB::StorageUpdateInterface& pkb) const override;
	[[nodiscard]] bool equals(const shared_ptr<StatementNode>& object) const override;

	static unique_ptr<CallNode> parseCallStatement(Lexer& lex, StmtRef& statement_count);

private:
	ProcRef procedure;
};

#endif  // SPA_SRC_SP_NODE_CALLNODE_H

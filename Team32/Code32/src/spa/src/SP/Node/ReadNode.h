#ifndef SPA_SRC_SP_NODE_READNODE_H
#define SPA_SRC_SP_NODE_READNODE_H

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "SP/Lexer.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

using namespace std;

class SP::Node::ReadNode : public StatementNode {
public:
	ReadNode(StmtRef stmt_no, unique_ptr<VariableNode> variable);
	StmtRef extract(PKB::StorageUpdateInterface& pkb) override;
	bool equals(const shared_ptr<StatementNode>& object) override;
	static unique_ptr<ReadNode> parseReadStatement(Lexer& lex, StmtRef& statement_count);

private:
	shared_ptr<VariableNode> variable;
};

#endif  // SPA_SRC_SP_NODE_READNODE_H

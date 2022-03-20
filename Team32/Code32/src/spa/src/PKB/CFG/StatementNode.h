#ifndef SPA_SRC_PKB_CFG_STATEMENTNODE_H
#define SPA_SRC_PKB_CFG_STATEMENTNODE_H

#include <utility>

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::StatementNode : public PKB::NodeInterface, public enable_shared_from_this<StatementNode> {
public:
	StatementNode(NodeType type, shared_ptr<StmtInfo> info);
	void setConnection(shared_ptr<NodeInterface> next) override;

	shared_ptr<StmtInfo> getStmtInfo() { return this->stmt_info; };
	StmtRef getNodeRef() override;
	~StatementNode() override = default;

private:
	shared_ptr<StmtInfo> stmt_info;
};

#endif  // SPA_SRC_PKB_CFG_STATEMENTNODE_H

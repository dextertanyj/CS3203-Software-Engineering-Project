#ifndef SPA_SRC_PKB_CFG_STATEMENTNODE_H
#define SPA_SRC_PKB_CFG_STATEMENTNODE_H

#include <utility>

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::StatementNode : public PKB::NodeInterface, public enable_shared_from_this<StatementNode> {
public:
	StatementNode(const shared_ptr<StmtInfo>& info);
	StatementNode(NodeType type, const shared_ptr<StmtInfo>& info);
	void setConnection(shared_ptr<NodeInterface> next) override;
	void setPrevious(const shared_ptr<NodeInterface>& prev) override;
	void setNext(const shared_ptr<NodeInterface>& next) override;

	[[nodiscard]] shared_ptr<StmtInfo> getStmtInfo() const;
	StmtRef getNodeRef() const override;
	~StatementNode() override = default;

	bool operator<(const StatementNode& other) const;
	bool operator>(const StatementNode& other) const;

private:
	shared_ptr<StmtInfo> stmt_info;
};

#endif  // SPA_SRC_PKB_CFG_STATEMENTNODE_H

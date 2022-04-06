#ifndef SPA_SRC_PKB_CFG_STATEMENTNODE_H
#define SPA_SRC_PKB_CFG_STATEMENTNODE_H

#include <cassert>
#include <utility>

#include "PKB/CFG/NodeInterface.h"
#include "PKB/PKB.h"

class PKB::StatementNode : public PKB::NodeInterface, public std::enable_shared_from_this<StatementNode> {
public:
	explicit StatementNode(const std::shared_ptr<StmtInfo>& info);
	StatementNode(Types::NodeType type, const std::shared_ptr<StmtInfo>& info);
	void setConnection(std::shared_ptr<NodeInterface> next) override;
	void setPrevious(const std::shared_ptr<NodeInterface>& prev) override;
	void setNext(const std::shared_ptr<NodeInterface>& next) override;

	[[nodiscard]] std::shared_ptr<StmtInfo> getStmtInfo() const;
	StmtRef getNodeRef() const override;
	~StatementNode() override = default;

private:
	std::shared_ptr<StmtInfo> stmt_info;
};

#endif  // SPA_SRC_PKB_CFG_STATEMENTNODE_H

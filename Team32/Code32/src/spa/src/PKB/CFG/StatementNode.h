#ifndef SPA_SRC_PKB_CFG_STATEMENTNODE_H
#define SPA_SRC_PKB_CFG_STATEMENTNODE_H

#include <cassert>
#include <utility>

#include "PKB/CFG/NodeInterface.h"
#include "PKB/PKB.h"

class PKB::StatementNode : public PKB::NodeInterface, public std::enable_shared_from_this<StatementNode> {
public:
	explicit StatementNode(const StmtInfoPtr& info);
	StatementNode(Types::NodeType type, StmtInfoPtr info);
	void setConnection(std::shared_ptr<NodeInterface> next) override;
	void setPrevious(const std::shared_ptr<NodeInterface>& prev) override;
	void setNext(const std::shared_ptr<NodeInterface>& next) override;

	[[nodiscard]] StmtInfoPtr getStmtInfo() const;
	StmtRef getNodeRef() const override;
	~StatementNode() override = default;

private:
	StmtInfoPtr stmt_info;
};

#endif  // SPA_SRC_PKB_CFG_STATEMENTNODE_H

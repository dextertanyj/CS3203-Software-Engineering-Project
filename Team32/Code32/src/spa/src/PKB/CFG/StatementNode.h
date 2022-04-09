#ifndef SPA_SRC_PKB_CFG_STATEMENTNODE_H
#define SPA_SRC_PKB_CFG_STATEMENTNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/CFG/NodeInterface.h"
#include "PKB/PKB.h"
#include "PKB/Types.h"

class PKB::StatementNode : public NodeInterface, public std::enable_shared_from_this<StatementNode> {
public:
	explicit StatementNode(const StmtInfoPtr& info);
	StatementNode(Types::NodeType type, StmtInfoPtr info);
	void setConnection(std::shared_ptr<NodeInterface> next) override;
	void setNext(const std::shared_ptr<NodeInterface>& next) override;
	void setPrevious(const std::shared_ptr<NodeInterface>& prev) override;

	[[nodiscard]] StmtInfoPtr getStmtInfo() const;
	StmtRef getNodeRef() const override;
	~StatementNode() override = default;

private:
	StmtInfoPtr stmt_info;
};

#endif  // SPA_SRC_PKB_CFG_STATEMENTNODE_H

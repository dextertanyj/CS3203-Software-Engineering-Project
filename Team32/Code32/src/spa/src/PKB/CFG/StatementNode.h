#ifndef SPA_SRC_PKB_CFG_STATEMENTNODE_H
#define SPA_SRC_PKB_CFG_STATEMENTNODE_H

#include <cassert>
#include <utility>

#include "PKB/CFG/NodeInterface.h"
#include "PKB/PKB.h"

class PKB::StatementNode : public PKB::NodeInterface, public enable_shared_from_this<StatementNode> {
public:
	explicit StatementNode(const shared_ptr<StmtInfo>& info);
	StatementNode(Types::NodeType type, const shared_ptr<StmtInfo>& info);
	void setConnection(shared_ptr<NodeInterface> next) override;
	void setPrevious(const shared_ptr<NodeInterface>& prev) override;
	void setNext(const shared_ptr<NodeInterface>& next) override;

	[[nodiscard]] shared_ptr<StmtInfo> getStmtInfo() const;
	StmtRef getNodeRef() const override;
	~StatementNode() override = default;

private:
	shared_ptr<StmtInfo> stmt_info;
};

#endif  // SPA_SRC_PKB_CFG_STATEMENTNODE_H

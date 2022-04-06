#ifndef SPA_SRC_PKB_CFG_IFNODE_H
#define SPA_SRC_PKB_CFG_IFNODE_H

#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"

class PKB::IfNode : public PKB::StatementNode {
public:
	explicit IfNode(const std::shared_ptr<StmtInfo>& info);
	void setConnection(std::shared_ptr<PKB::NodeInterface> next) override;
	void setNext(const std::shared_ptr<PKB::NodeInterface>& next) override;
	void setPrevious(const std::shared_ptr<PKB::NodeInterface>& previous) override;

	void insertIfNext(const std::shared_ptr<PKB::StatementNode>& first, const std::shared_ptr<PKB::StatementNode>& second);
	void insertIfExit(const std::shared_ptr<PKB::StatementNode>& first, const std::shared_ptr<PKB::StatementNode>& second);

private:
	std::shared_ptr<PKB::DummyNode> dummy_node;
};

#endif  // SPA_SRC_PKB_CFG_IFNODE_H

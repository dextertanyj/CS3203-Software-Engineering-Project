#ifndef SPA_SRC_PKB_CFG_IFNODE_H
#define SPA_SRC_PKB_CFG_IFNODE_H

#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"

class PKB::IfNode : public PKB::StatementNode {
public:
	explicit IfNode(const shared_ptr<StmtInfo>& info);
	void setConnection(shared_ptr<PKB::NodeInterface> next) override;
	void setNext(const shared_ptr<PKB::NodeInterface>& next) override;
	void setPrevious(const shared_ptr<PKB::NodeInterface>& previous) override;

	void insertIfNext(const shared_ptr<PKB::StatementNode>& first, const shared_ptr<PKB::StatementNode>& second);
	void insertIfExit(const shared_ptr<PKB::StatementNode>& first, const shared_ptr<PKB::StatementNode>& second);

private:
	shared_ptr<PKB::DummyNode> dummy_node;
};

#endif  // SPA_SRC_PKB_CFG_IFNODE_H

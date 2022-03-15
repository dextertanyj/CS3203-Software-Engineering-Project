#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include "PKB/PKB.h"
#include "StatementNode.h"

class PKB::IfNode : public PKB::StatementNode {
public:
	IfNode(shared_ptr<StmtInfo> info);
	size_t getNodeRef() override;
	shared_ptr<PKB::NodeInterface> getDummyNode();
	void insertNext(shared_ptr<PKB::NodeInterface> next) override;
	void insertPrevious(shared_ptr<PKB::NodeInterface> prev) override;
	void insertIfNext(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second, shared_ptr<PKB::IfNode> self);
	void insertIfExit(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second);

private:
	shared_ptr<PKB::DummyNode> dummy_node;
};

#endif  // SPA_IFNODE_H

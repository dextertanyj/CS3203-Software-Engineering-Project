#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::IfNode : public PKB::NodeInterface {
public:
	IfNode(shared_ptr<StmtInfo> info);
	size_t getNodeRef() override;
	shared_ptr<PKB::NodeInterface> getDummyNode();
	void setDummyNode(shared_ptr<PKB::NodeInterface> to_insert);
	void insertNext(shared_ptr<PKB::NodeInterface> next) override;
	void insertPrevious(shared_ptr<PKB::NodeInterface> prev, bool to_dummy = true) override;
	void insertIfNext(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second, shared_ptr<PKB::IfNode> self);
	void insertIfExit(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second);

private:
	shared_ptr<StmtInfo> statement_info;
	shared_ptr<PKB::NodeInterface> dummy_node;
};

#endif  // SPA_IFNODE_H

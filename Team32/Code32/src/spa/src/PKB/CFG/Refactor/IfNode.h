#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::IfNode : public PKB::NodeInterface {
public:
	IfNode(shared_ptr<StmtInfo> info);
	shared_ptr<PKB::NodeInterface> getDummyNode() override;
	void setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) override;
	size_t getNodeRef() override;

private:
	shared_ptr<StmtInfo> statement_info;
	shared_ptr<PKB::NodeInterface> dummy_node;
};

#endif  // SPA_IFNODE_H

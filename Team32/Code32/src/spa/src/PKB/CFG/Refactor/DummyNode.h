#ifndef SPA_DUMMYNODE_H
#define SPA_DUMMYNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::DummyNode : public PKB::NodeInterface {
public:
	DummyNode(size_t ref);
	shared_ptr<PKB::NodeInterface> getDummyNode() override;
	void setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) override;
	size_t getNodeRef() override;

private:
	size_t if_control_stmt_ref;
};

#endif  // SPA_DUMMYNODE_H

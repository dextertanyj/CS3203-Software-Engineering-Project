#ifndef SPA_DUMMYNODE_H
#define SPA_DUMMYNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::DummyNode : public PKB::NodeInterface {
public:
	DummyNode(size_t ref);
	shared_ptr<PKB::NodeInterface> getDummyNode() override;
	void setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) override;
};

#endif  // SPA_DUMMYNODE_H

#ifndef SPA_DUMMYNODE_H
#define SPA_DUMMYNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::DummyNode : public PKB::NodeInterface {
public:
	DummyNode(size_t ref);
};

#endif  // SPA_DUMMYNODE_H

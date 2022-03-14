#ifndef SPA_DUMMYNODE_H
#define SPA_DUMMYNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::DummyNode : public PKB::NodeInterface {
public:
	DummyNode(size_t ref);
	size_t getNodeRef() override;
	void insertNext(shared_ptr<PKB::NodeInterface> next) override;
	void insertPrevious(shared_ptr<PKB::NodeInterface> prev, bool to_dummy = true) override;

private:
	size_t if_control_stmt_ref;
};

#endif  // SPA_DUMMYNODE_H

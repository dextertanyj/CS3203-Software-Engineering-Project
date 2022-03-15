#ifndef SPA_DUMMYNODE_H
#define SPA_DUMMYNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::DummyNode : public PKB::NodeInterface, public enable_shared_from_this<DummyNode> {
public:
	explicit DummyNode(StmtRef ref);
	void setConnection(shared_ptr<NodeInterface> next) override;
	void setNext(shared_ptr<PKB::NodeInterface> next) override;
	void setPrevious(shared_ptr<PKB::NodeInterface> prev) override;

	StmtRef getNodeRef() override;
	~DummyNode() override = default;

private:
	StmtRef if_control_stmt_ref;
};

#endif  // SPA_DUMMYNODE_H
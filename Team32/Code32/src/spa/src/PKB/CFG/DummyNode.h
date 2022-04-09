#ifndef SPA_SRC_PKB_CFG_DUMMYNODE_H
#define SPA_SRC_PKB_CFG_DUMMYNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/CFG/CFG.h"
#include "PKB/CFG/NodeInterface.h"

class PKB::CFG::DummyNode : public NodeInterface, public std::enable_shared_from_this<DummyNode> {
public:
	explicit DummyNode(StmtRef index);
	void setConnection(std::shared_ptr<NodeInterface> next) override;
	void setNext(const std::shared_ptr<NodeInterface>& next) override;
	void setPrevious(const std::shared_ptr<NodeInterface>& prev) override;

	StmtRef getNodeRef() const override;
	~DummyNode() override = default;

private:
	StmtRef if_control_stmt_ref;
};

#endif  // SPA_SRC_PKB_CFG_DUMMYNODE_H

#ifndef SPA_SRC_PKB_CFG_IFNODE_H
#define SPA_SRC_PKB_CFG_IFNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/CFG/DummyNode.h"
#include "PKB/CFG/NodeInterface.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"

class PKB::IfNode : public StatementNode {
public:
	explicit IfNode(const StmtInfoPtr& info);
	void setConnection(std::shared_ptr<NodeInterface> next) override;
	void setNext(const std::shared_ptr<NodeInterface>& next) override;
	void setPrevious(const std::shared_ptr<NodeInterface>& prev) override;

	void insertIfNext(const std::shared_ptr<StatementNode>& first, const std::shared_ptr<StatementNode>& second);
	void insertIfExit(const std::shared_ptr<StatementNode>& first, const std::shared_ptr<StatementNode>& second);

private:
	std::shared_ptr<DummyNode> dummy_node;
};

#endif  // SPA_SRC_PKB_CFG_IFNODE_H

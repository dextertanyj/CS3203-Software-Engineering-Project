#ifndef SPA_SRC_PKB_CFG_WHILENODE_H
#define SPA_SRC_PKB_CFG_WHILENODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "PKB/CFG/NodeInterface.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"

class PKB::WhileNode : public StatementNode {
public:
	explicit WhileNode(const StmtInfoPtr& info);
	void setNext(const std::shared_ptr<NodeInterface>& next) override;
	void setPrevious(const std::shared_ptr<NodeInterface>& prev) override;
};

#endif  // SPA_SRC_PKB_CFG_WHILENODE_H

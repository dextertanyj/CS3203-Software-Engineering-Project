#ifndef SPA_SRC_PKB_CFG_WHILENODE_H
#define SPA_SRC_PKB_CFG_WHILENODE_H

#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"

class PKB::WhileNode : public PKB::StatementNode {
public:
	explicit WhileNode(const shared_ptr<StmtInfo>& info);
	void setNext(const shared_ptr<NodeInterface>& next) override;
	void setPrevious(const shared_ptr<NodeInterface>& prev) override;
};

#endif  // SPA_SRC_PKB_CFG_WHILENODE_H

#ifndef SPA_SRC_PKB_CFG_NONCONDITIONALNODE_H
#define SPA_SRC_PKB_CFG_NONCONDITIONALNODE_H

#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"

class PKB::NonConditionalNode : public PKB::StatementNode {
public:
	explicit NonConditionalNode(const shared_ptr<StmtInfo>& info);
	void setPrevious(const shared_ptr<NodeInterface>& prev) override;
	void setNext(const shared_ptr<NodeInterface>& next) override;
};
#endif  // SPA_SRC_PKB_CFG_NONCONDITIONALNODE_H

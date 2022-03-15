#ifndef SPA_NONCONDITIONALNODE_H
#define SPA_NONCONDITIONALNODE_H

#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"

class PKB::NonConditionalNode : public PKB::StatementNode {
public:
	explicit NonConditionalNode(shared_ptr<StmtInfo> info);
	void setPrevious(shared_ptr<NodeInterface> prev) override;
	void setNext(shared_ptr<NodeInterface> next) override;
};
#endif  // SPA_NONCONDITIONALNODE_H

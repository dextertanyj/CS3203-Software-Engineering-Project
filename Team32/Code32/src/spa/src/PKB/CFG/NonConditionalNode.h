#ifndef SPA_NONCONDITIONALNODE_H
#define SPA_NONCONDITIONALNODE_H

#include "PKB/PKB.h"
#include "StatementNode.h"

class PKB::NonConditionalNode : public PKB::StatementNode {
public:
	NonConditionalNode(shared_ptr<StmtInfo> info);
	size_t getNodeRef() override;
	void insertNext(shared_ptr<PKB::NodeInterface> next) override;
	void insertPrevious(shared_ptr<PKB::NodeInterface> prev, bool to_dummy = true) override;
};
#endif  // SPA_NONCONDITIONALNODE_H

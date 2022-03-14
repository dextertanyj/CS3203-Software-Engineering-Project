#ifndef SPA_NONCONDITIONALNODE_H
#define SPA_NONCONDITIONALNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::NonConditionalNode : public PKB::NodeInterface {
public:
	NonConditionalNode(shared_ptr<StmtInfo> info);
	size_t getNodeRef() override;

private:
	shared_ptr<StmtInfo> statement_info;
};
#endif  // SPA_NONCONDITIONALNODE_H

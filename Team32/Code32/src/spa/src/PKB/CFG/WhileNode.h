#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include "PKB/PKB.h"
#include "StatementNode.h"

class PKB::WhileNode : public PKB::StatementNode {
public:
	WhileNode(shared_ptr<StmtInfo> info);
	size_t getNodeRef() override;
	void insertNext(shared_ptr<PKB::NodeInterface> next) override;
	void insertPrevious(shared_ptr<PKB::NodeInterface> prev, bool to_dummy = true) override;
};
#endif  // SPA_WHILENODE_H

#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"

class PKB::WhileNode : public PKB::StatementNode {
public:
	explicit WhileNode(const shared_ptr<StmtInfo>& info);
	void setNext(shared_ptr<NodeInterface> next) override;
	void setPrevious(shared_ptr<NodeInterface> prev) override;
};

#endif  // SPA_WHILENODE_H

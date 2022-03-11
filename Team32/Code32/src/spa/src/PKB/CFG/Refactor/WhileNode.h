#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::WhileNode : public PKB::NodeInterface {
public:
	WhileNode(shared_ptr<StmtInfo> info);
	shared_ptr<PKB::NodeInterface> getDummyNode() override;
	void setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) override;

private:
	shared_ptr<StmtInfo> statement_info;
};
#endif  // SPA_WHILENODE_H

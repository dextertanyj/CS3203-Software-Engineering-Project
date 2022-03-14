#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::WhileNode : public PKB::NodeInterface {
public:
	WhileNode(shared_ptr<StmtInfo> info);
	size_t getNodeRef() override;
	void insertNext(shared_ptr<PKB::NodeInterface> next) override;
	void insertPrevious(shared_ptr<PKB::NodeInterface> prev, bool to_dummy = true) override;

private:
	shared_ptr<StmtInfo> statement_info;
};
#endif  // SPA_WHILENODE_H

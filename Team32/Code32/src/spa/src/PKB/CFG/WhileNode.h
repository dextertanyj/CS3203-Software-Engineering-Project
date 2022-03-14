#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::WhileNode : public PKB::NodeInterface {
public:
	WhileNode(shared_ptr<StmtInfo> info);
	size_t getNodeRef() override;
	~WhileNode(){};

private:
	shared_ptr<StmtInfo> statement_info;
};
#endif  // SPA_WHILENODE_H

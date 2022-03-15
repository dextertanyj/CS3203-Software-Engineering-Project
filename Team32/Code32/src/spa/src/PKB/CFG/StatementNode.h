#ifndef SPA_STATEMENTNODE_H
#define SPA_STATEMENTNODE_H

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::StatementNode : public PKB::NodeInterface {
public:
	StatementNode(shared_ptr<StmtInfo> info) { this->stmt_info = info; }
	shared_ptr<StmtInfo> getStmtInfo() { return this->stmt_info; };
	virtual void insertNext(shared_ptr<PKB::NodeInterface> next) = 0;
	virtual void insertPrevious(shared_ptr<PKB::NodeInterface> prev) = 0;
	virtual size_t getNodeRef() = 0;

protected:
	shared_ptr<StmtInfo> stmt_info;
};

#endif  // SPA_STATEMENTNODE_H

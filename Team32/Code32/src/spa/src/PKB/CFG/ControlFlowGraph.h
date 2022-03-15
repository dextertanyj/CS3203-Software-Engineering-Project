#ifndef SPA_CONTROLFLOWGRAPH_H
#define SPA_CONTROLFLOWGRAPH_H

#include <set>
#include <unordered_map>
#include <queue>

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::ControlFlowGraph {
public:
	void createNode(shared_ptr<StmtInfo> stmt_info);
	bool checkNext(StmtRef prev, StmtRef next);
	bool checkNextStar(StmtRef i, StmtRef i1);
	StmtInfoPtrSet getNextNodes(StmtRef ref);
	StmtInfoPtrSet getPreviousNodes(StmtRef ref);
	StmtInfoPtrSet getNextStarNodes(StmtRef ref);
	StmtInfoPtrSet getPreviousStarNodes(StmtRef ref);
	void setNext(StmtRef prev, StmtRef next);
	void setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next);
	void setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref);
	void clear();
	void optimize();

private:
	void populateGraphIndex();
	shared_ptr<StmtInfo> collectNextOfDummy(shared_ptr<PKB::NodeInterface> dummy_node);
	StmtInfoPtrSet collectPreviousOfDummy(shared_ptr<PKB::NodeInterface> dummy_node);
	shared_ptr<PKB::NodeInterface> getNode(StmtRef ref);
	unordered_map<StmtRef, shared_ptr<PKB::NodeInterface>> stmt_to_normal_node_store;
	// TODO: Next* Query Cache can be built into this class in the future.
};

#endif  // SPA_CONTROLFLOWGRAPH_H

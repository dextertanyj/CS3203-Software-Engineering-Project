#ifndef SPA_CONTROLFLOWGRAPH_H
#define SPA_CONTROLFLOWGRAPH_H

#include <set>
#include <unordered_map>

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::ControlFlowGraph {
public:
	void createNode(shared_ptr<StmtInfo> stmt_info);
	bool checkNext(StmtRef prev, StmtRef next);
	bool checkNextStar(StmtRef i, StmtRef i1);
	StmtRefSet getNextNodes(StmtRef ref);
	StmtRefSet getPreviousNodes(StmtRef ref);
	void setNext(StmtRef prev, StmtRef next);
	void setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next);
	void setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref);
	void clear();
	void optimize();

private:
	void populateUniqueIndex();
	shared_ptr<PKB::NodeInterface> getNode(StmtRef ref);
	void setConnection(shared_ptr<PKB::NodeInterface> prev, shared_ptr<PKB::NodeInterface> next);
	void createDummyNode(shared_ptr<PKB::IfNode> if_node);
	unordered_map<StmtRef, shared_ptr<PKB::NodeInterface>> stmt_to_normal_node_store;
	unordered_map<StmtRef, shared_ptr<PKB::NodeInterface>> stmt_to_dummy_node_store;
	// TODO: Next* Query Cache can be built into this class in the future.
};

#endif  // SPA_CONTROLFLOWGRAPH_H

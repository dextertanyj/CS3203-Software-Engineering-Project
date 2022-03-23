#ifndef SPA_SRC_PKB_CFG_CONTROLFLOWGRAPH_H
#define SPA_SRC_PKB_CFG_CONTROLFLOWGRAPH_H

#include <queue>
#include <set>
#include <unordered_map>

#include "NodeInterface.h"
#include "PKB/PKB.h"
#include "PKB/Types.h"

class PKB::ControlFlowGraph {
public:
	void createNode(shared_ptr<StmtInfo> stmt_info);
	void clear();
	void optimize();

protected:
	bool checkNext(StmtRef prev, StmtRef next);
	bool checkNextStar(StmtRef i, StmtRef i1);
	StmtInfoPtrSet getNextNodes(StmtRef ref);
	StmtInfoPtrSet getPreviousNodes(StmtRef ref);
	StmtInfoPtrSet getNextStarNodes(StmtRef ref);
	StmtInfoPtrSet getPreviousStarNodes(StmtRef ref);
	void setNext(StmtRef prev, StmtRef next);
	void setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next);
	void setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref);

	friend class NextManager;
	friend class AffectsManager;

private:
	void populateGraphIndex();
	StmtInfoPtrSet collectNextOfDummy(const shared_ptr<NodeInterface>& dummy_node);
	StmtInfoPtrSet collectPreviousOfDummy(const shared_ptr<NodeInterface>& dummy_node);
	shared_ptr<StatementNode> getNode(StmtRef ref);
	unordered_map<StmtRef, shared_ptr<StatementNode>> stmt_to_normal_node_store;
	void processBFSVisit(Types::BFSInfo& info, const shared_ptr<NodeInterface>& node,
	                     StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface>&));
};

#endif  // SPA_SRC_PKB_CFG_CONTROLFLOWGRAPH_H

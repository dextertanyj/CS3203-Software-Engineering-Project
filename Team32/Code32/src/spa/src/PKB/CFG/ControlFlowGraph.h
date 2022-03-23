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

private:
	void populateGraphIndex();
	StmtInfoPtrSet collectNextOfDummy(const shared_ptr<NodeInterface>& dummy_node);
	StmtInfoPtrSet collectPreviousOfDummy(const shared_ptr<NodeInterface>& dummy_node);
	shared_ptr<StatementNode> getNode(StmtRef ref);
	unordered_map<StmtRef, shared_ptr<StatementNode>> stmt_to_normal_node_store;

	friend class NextManager;
	friend class AffectsManager;
};

#endif  // SPA_SRC_PKB_CFG_CONTROLFLOWGRAPH_H

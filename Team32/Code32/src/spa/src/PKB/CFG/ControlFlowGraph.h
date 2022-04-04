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
	shared_ptr<StatementNode> getNode(StmtRef ref);
	size_t getGraphIndex(StmtRef ref);
	StmtRef getFirstIndex(size_t graph_index);
	StmtRef getLastIndex(size_t graph_index);
	shared_ptr<NodeInterface> getStart(size_t graph_index);
	shared_ptr<NodeInterface> getEnd(size_t graph_index);
	void clear();
	void optimize();

	static StmtInfoPtrSet collectNextOfDummy(const shared_ptr<NodeInterface>& dummy_node);
	static StmtInfoPtrSet collectPreviousOfDummy(const shared_ptr<NodeInterface>& dummy_node);

private:
	void processGraphNode(const shared_ptr<NodeInterface>& node, size_t graph_index, StmtRef& last,
	                      unordered_set<shared_ptr<NodeInterface>>& visited);
	unordered_map<StmtRef, shared_ptr<StatementNode>> statement_node_map;
	unordered_map<size_t, StmtRef> first_index_map;
	unordered_map<size_t, StmtRef> last_index_map;
	unordered_map<size_t, shared_ptr<NodeInterface>> start_map;
	unordered_map<size_t, shared_ptr<NodeInterface>> end_map;
};

#endif  // SPA_SRC_PKB_CFG_CONTROLFLOWGRAPH_H

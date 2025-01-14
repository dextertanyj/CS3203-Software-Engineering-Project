#ifndef SPA_SRC_PKB_CFG_CONTROLFLOWGRAPH_H
#define SPA_SRC_PKB_CFG_CONTROLFLOWGRAPH_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Common/TypeDefs.h"
#include "PKB/CFG/CFG.h"
#include "PKB/CFG/NodeInterface.h"
#include "PKB/CFG/StatementNode.h"

class PKB::CFG::ControlFlowGraph {
public:
	void createNode(const StmtInfoPtr& stmt_info);
	void setNext(StmtRef prev, StmtRef next) const;
	void setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) const;
	void setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) const;

	[[nodiscard]] bool contains(StmtRef index) const;
	[[nodiscard]] StmtType getType(StmtRef index) const;
	[[nodiscard]] StmtInfoPtr getStmtInfo(StmtRef index) const;

	[[nodiscard]] StmtInfoPtrSet getNextNodes(StmtRef index) const;
	[[nodiscard]] StmtInfoPtrSet getPreviousNodes(StmtRef index) const;
	[[nodiscard]] StmtInfoPtrSet getLoopExternalNextNodes(StmtRef index) const;
	[[nodiscard]] StmtInfoPtrSet getLoopExternalPreviousNodes(StmtRef index) const;
	[[nodiscard]] StmtInfoPtrSet getLoopInternalNextNodes(StmtRef index) const;
	[[nodiscard]] StmtInfoPtrSet getLoopInternalPreviousNodes(StmtRef index) const;

	[[nodiscard]] size_t getGraphIndex(StmtRef index) const;
	[[nodiscard]] StmtRef getFirstIndex(size_t graph_index) const;
	[[nodiscard]] StmtRef getLastIndex(size_t graph_index) const;

	void optimize();
	void clear();

	static StmtInfoPtrSet collectNextOfDummy(const std::shared_ptr<NodeInterface>& dummy_node);
	static StmtInfoPtrSet collectPreviousOfDummy(const std::shared_ptr<NodeInterface>& dummy_node);

private:
	using LoopNodePair = std::pair<StmtInfoPtrSet, StmtInfoPtrSet>;
	using NodeGatherer = std::unordered_set<std::shared_ptr<NodeInterface>> (NodeInterface::*)() const;
	using Collector = StmtInfoPtrSet (*)(const std::shared_ptr<NodeInterface>&);

	std::unordered_map<StmtRef, std::shared_ptr<StatementNode>> statement_node_map;
	std::unordered_map<size_t, StmtRef> first_index_map;
	std::unordered_map<size_t, StmtRef> last_index_map;

	[[nodiscard]] std::shared_ptr<StatementNode> getNode(StmtRef index) const;

	void processGraphNode(const std::shared_ptr<NodeInterface>& node, size_t graph_index, StmtRef& last,
	                      std::unordered_set<std::shared_ptr<NodeInterface>>& visited);

	template <typename Comparator>
	LoopNodePair groupLoopNeighbouringNodes(StmtRef index, NodeGatherer gatherer, Collector collector) const;

	StmtInfoPtrSet collectLoopNeighbours(const std::shared_ptr<NodeInterface>& node, Collector collector) const;

	template <class T>
	void handleDummyNodeSearch(T& queue, const std::shared_ptr<NodeInterface>& dummy_node, Collector collector) const;
};

#endif  // SPA_SRC_PKB_CFG_CONTROLFLOWGRAPH_H

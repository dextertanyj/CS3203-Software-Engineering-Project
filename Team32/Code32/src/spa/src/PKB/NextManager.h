#ifndef SPA_SRC_PKB_NEXTMANAGER_H
#define SPA_SRC_PKB_NEXTMANAGER_H

#include <queue>
#include <unordered_map>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/CFG/ControlFlowGraph.h"
#include "PKB/PKB.h"

class PKB::NextManager {
public:
	explicit NextManager(ControlFlowGraph& control_flow_graph);

	bool checkNext(StmtRef first, StmtRef second);
	bool checkNextStar(StmtRef first, StmtRef second);
	StmtInfoPtrSet getNext(StmtRef node_ref);
	StmtInfoPtrSet getNextStar(StmtRef node_ref);
	StmtInfoPtrSet getPrevious(StmtRef node_ref);
	StmtInfoPtrSet getPreviousStar(StmtRef node_ref);

	void resetCache();

private:
	// Type definitions
	struct TraversalInformation {
		std::unordered_map<StmtRef, StmtInfoPtrSet>& cache;
		StmtInfoPtrSet (ControlFlowGraph::*gatherer)(StmtRef) const;
		StmtInfoPtrSet (ControlFlowGraph::*loop_continuation_handler)(StmtRef) const;
	};

	template <class Comparator>
	using StmtInfoPQ = std::priority_queue<StmtInfoPtr, std::vector<StmtInfoPtr>, Comparator>;

	template <class Comparator>
	struct QueueConstructionInformation {
		StmtInfoPtr origin;
		StmtInfoPQ<Comparator>& priority_queue;
		std::queue<StmtInfoPtr>& queue;
		TraversalInformation& traversal_information;
	};

	bool checkNextStarOptimized(StmtRef first_node, StmtRef second_node);

	template <class Comparator>
	StmtInfoPQ<Comparator> constructQueue(const StmtInfoPtr& origin, TraversalInformation& info);
	template <class Comparator>
	void constructQueueIteration(const StmtInfoPtr& node, QueueConstructionInformation<Comparator>& info);
	void processQueue(const StmtInfoPtr& node, TraversalInformation& info);

	// Loop node optimizations
	template <class Comparator>
	void constructQueueLoopNode(const StmtInfoPtr& node, QueueConstructionInformation<Comparator>& info);
	void processLoopNode(const StmtInfoPtr& node, TraversalInformation& info);
	StmtInfoPtrSet traverseLoop(const StmtInfoPtr& node);
	void handleTraverseLoopNode(std::queue<StmtInfoPtr>& queue, StmtInfoPtrSet& set, const StmtInfoPtr& node);

	// Members
	ControlFlowGraph& control_flow_graph;
	std::unordered_map<StmtRef, StmtInfoPtrSet> next_cache;
	std::unordered_map<StmtRef, StmtInfoPtrSet> previous_cache;
};

#endif  // SPA_SRC_PKB_NEXTMANAGER_H

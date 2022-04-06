#ifndef SPA_NEXTMANAGER_H
#define SPA_NEXTMANAGER_H

#include "PKB/CFG/ControlFlowGraph.h"
#include "PKB/CFG/IfNode.h"
#include "PKB/CFG/NodeInterface.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"
#include "PKB/Types.h"

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
	using StmtInfoPQ = std::priority_queue<std::shared_ptr<StmtInfo>, std::vector<std::shared_ptr<StmtInfo>>, Comparator>;

	template <class Comparator>
	struct QueueConstructionInformation {
		std::shared_ptr<StmtInfo> origin;
		StmtInfoPQ<Comparator>& priority_queue;
		std::queue<std::shared_ptr<StmtInfo>>& queue;
		TraversalInformation& traversal_information;
	};

	bool checkNextStarOptimized(const StmtRef& first_node, const StmtRef& second_node);

	template <class Comparator>
	StmtInfoPQ<Comparator> constructQueue(const std::shared_ptr<StmtInfo>& origin, TraversalInformation& info);
	template <class Comparator>
	void constructQueueIteration(const std::shared_ptr<StmtInfo>& node, QueueConstructionInformation<Comparator>& info);
	void processQueue(const std::shared_ptr<StmtInfo>& node, TraversalInformation& info);

	// Loop node optimizations
	template <class Comparator>
	void constructQueueLoopNode(const std::shared_ptr<StmtInfo>& node, QueueConstructionInformation<Comparator>& info);
	void processLoopNode(const std::shared_ptr<StmtInfo>& node, TraversalInformation& info);
	StmtInfoPtrSet traverseLoop(const std::shared_ptr<StmtInfo>& node);
	void handleTraverseLoopNode(std::queue<std::shared_ptr<StmtInfo>>& queue, StmtInfoPtrSet& set, const std::shared_ptr<StmtInfo>& node);

	// Members
	ControlFlowGraph& control_flow_graph;
	std::unordered_map<StmtRef, StmtInfoPtrSet> next_cache;
	std::unordered_map<StmtRef, StmtInfoPtrSet> previous_cache;
};

#endif  // SPA_NEXTMANAGER_H

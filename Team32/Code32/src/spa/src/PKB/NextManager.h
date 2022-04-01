#ifndef SPA_NEXTMANAGER_H
#define SPA_NEXTMANAGER_H

#include "PKB/CFG/ControlFlowGraph.h"
#include "PKB/CFG/IfNode.h"
#include "PKB/CFG/NodeInterface.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"
#include "PKB/Types.h"

using namespace std;

class PKB::NextManager {
public:
	explicit NextManager(ControlFlowGraph& control_flow_graph);
	void setNext(StmtRef previous, StmtRef next);
	void setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next);
	void setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref);

	bool checkNext(StmtRef first, StmtRef second);
	bool checkNextStar(StmtRef first, StmtRef second);
	StmtInfoPtrSet getNext(StmtRef node_ref);
	StmtInfoPtrSet getNextStar(StmtRef node_ref);
	StmtInfoPtrSet getPrevious(StmtRef node_ref);
	StmtInfoPtrSet getPreviousStar(StmtRef node_ref);

	void resetCache();

	using LoopNodePair = pair<unordered_set<shared_ptr<StatementNode>>, unordered_set<shared_ptr<StatementNode>>>;

private:
	// Type definitions
	struct TraversalInformation {
		unordered_map<StmtRef, StmtInfoPtrSet>& cache;
		unordered_set<shared_ptr<NodeInterface>> (NodeInterface::*gatherer)() const;
		LoopNodePair (NextManager::*loop_continuation_handler)(const shared_ptr<StatementNode>&);
		StmtInfoPtrSet (*collector)(const shared_ptr<NodeInterface>&);
	};

	template <class Comparator>
	struct QueueConstructionInformation {
		shared_ptr<StatementNode> origin;
		priority_queue<shared_ptr<StatementNode>, vector<shared_ptr<StatementNode>>, Comparator>& priority_queue;
		queue<shared_ptr<StatementNode>>& queue;
		TraversalInformation& traversal_information;
	};

	bool checkNextStarOptimized(const shared_ptr<StatementNode>& first_node, const shared_ptr<StatementNode>& second_node);

	template <class Comparator>
	priority_queue<shared_ptr<StatementNode>, vector<shared_ptr<StatementNode>>, Comparator> constructQueue(
		const shared_ptr<StatementNode>& origin, TraversalInformation& info);
	template <class Comparator>
	void constructQueueIteration(const shared_ptr<StatementNode>& node, QueueConstructionInformation<Comparator>& info);
	void processQueue(const shared_ptr<StatementNode>& node, TraversalInformation& info);

	// Loop node optimizations
	template <class Comparator>
	void constructQueueLoopNode(const shared_ptr<StatementNode>& node, QueueConstructionInformation<Comparator>& info);
	void processLoopNode(const shared_ptr<StatementNode>& node, TraversalInformation& info);
	StmtInfoPtrSet traverseLoop(const shared_ptr<NodeInterface>& node);
	void handleTraverseLoopNode(queue<shared_ptr<NodeInterface>>& queue, StmtInfoPtrSet& set, const shared_ptr<NodeInterface>& node);

	// Loop entry-exit utilities
	template <typename Comparator>
	LoopNodePair processLoopEntryExit(const shared_ptr<StatementNode>& loop_node,
	                                  unordered_set<shared_ptr<NodeInterface>> (NodeInterface::*gatherer)() const,
	                                  StmtInfoPtrSet (*collector)(const shared_ptr<NodeInterface>&));
	LoopNodePair processLoopEntry(const shared_ptr<StatementNode>& node);
	LoopNodePair processLoopExit(const shared_ptr<StatementNode>& node);
	unordered_set<shared_ptr<StatementNode>> checkLoopNeighbour(const shared_ptr<NodeInterface>& node,
	                                                            StmtInfoPtrSet (*collector)(const shared_ptr<NodeInterface>&));

	template <class T>
	void handleDummyNodeSearch(T& queue, const shared_ptr<NodeInterface>& node,
	                           StmtInfoPtrSet (*collector)(const shared_ptr<NodeInterface>&));

	// Members
	ControlFlowGraph* control_flow_graph;
	unordered_map<StmtRef, StmtInfoPtrSet> next_cache;
	unordered_map<StmtRef, StmtInfoPtrSet> previous_cache;
};

#endif  // SPA_NEXTMANAGER_H

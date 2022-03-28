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

private:
	struct TraversalInformation {
		unordered_map<StmtRef, StmtInfoPtrSet>* cache;
		unordered_set<shared_ptr<PKB::NodeInterface>> (PKB::NodeInterface::*gatherer)() const;
		pair<unordered_set<shared_ptr<PKB::StatementNode>>, unordered_set<shared_ptr<PKB::StatementNode>>> (
			PKB::NextManager::*loop_continuation_handler)(const shared_ptr<PKB::StatementNode>&);
		StmtInfoPtrSet (PKB::ControlFlowGraph::*collector)(const shared_ptr<PKB::NodeInterface>&);
	};

	ControlFlowGraph* control_flow_graph;
	void processBFSVisit(Types::BFSInfo& info, const shared_ptr<NodeInterface>& node,
	                     StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface>&));
	StmtInfoPtrSet traverseLoop(const shared_ptr<NodeInterface>& node);

	template <class Comparator>
	priority_queue<shared_ptr<PKB::StatementNode>, vector<shared_ptr<PKB::StatementNode>>, Comparator> constructQueue(
		const shared_ptr<StatementNode>& node, TraversalInformation& info);
	void processQueue(const shared_ptr<StatementNode>& node, TraversalInformation& info);

	void processLoopNode(const shared_ptr<PKB::StatementNode>& node, TraversalInformation& info);

	template <typename Comparator>
	pair<unordered_set<shared_ptr<PKB::StatementNode>>, unordered_set<shared_ptr<PKB::StatementNode>>> processLoopEntryExit(
		const shared_ptr<PKB::StatementNode>& node, unordered_set<shared_ptr<PKB::NodeInterface>> (NodeInterface::*gatherer)() const,
		StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface>&));
	pair<unordered_set<shared_ptr<PKB::StatementNode>>, unordered_set<shared_ptr<PKB::StatementNode>>> processLoopExit(
		const shared_ptr<PKB::StatementNode>& node);
	pair<unordered_set<shared_ptr<PKB::StatementNode>>, unordered_set<shared_ptr<PKB::StatementNode>>> processLoopEntry(
		const shared_ptr<PKB::StatementNode>& node);

	template <class T>
	void handleDummyNodeSearch(T& queue, const shared_ptr<NodeInterface>& node,
	                           StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface>&));

	unordered_map<StmtRef, StmtInfoPtrSet> next_cache;
	unordered_map<StmtRef, StmtInfoPtrSet> previous_cache;
};

#endif  // SPA_NEXTMANAGER_H

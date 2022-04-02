#ifndef SPA_AFFECTSMANAGER_H
#define SPA_AFFECTSMANAGER_H

#include <unordered_map>

#include "PKB/InfoStore.h"
#include "PKB/CFG/ControlFlowGraph.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/ModifiesSRelation.h"
#include "PKB/NextManager.h"
#include "PKB/PKB.h"
#include "PKB/SVRelationStore.tpp"
#include "PKB/Types.h"
#include "PKB/UsesSRelation.h"

class PKB::AffectsManager {
public:
	explicit AffectsManager(ControlFlowGraph& control_flow_graph,
	                        SVRelationStore<PKB::ModifiesSRelation>& modifies_store,
	                        SVRelationStore<PKB::UsesSRelation>& uses_store);
	bool checkAffects(StmtRef first, StmtRef second);
	bool checkAffectsStar(StmtRef first, StmtRef second);
	StmtInfoPtrSet getAffects(StmtRef first);
	StmtInfoPtrSet getAffected(StmtRef second);
	StmtInfoPtrSet getAffectsStar(StmtRef node_ref);
	StmtInfoPtrSet getAffectedStar(StmtRef node_ref);
	void resetCache();

private:
	struct CacheGraphNode {
		StmtInfoPtrSet statements;
		bool strongly_connected;
		unordered_set<shared_ptr<CacheGraphNode>> affected;
		unordered_set<shared_ptr<CacheGraphNode>> affects;
		CacheGraphNode(StmtInfoPtrSet statements, bool strongly_connected) : statements(move(statements)), strongly_connected(strongly_connected) {};
	};

	ControlFlowGraph* control_flow_graph;
	SVRelationStore<PKB::UsesSRelation>* uses_store;
	SVRelationStore<PKB::ModifiesSRelation>* modifies_store;

	unordered_map<StmtRef, shared_ptr<CacheGraphNode>> cache_graph_store;
	unordered_map<StmtRef, StmtInfoPtrSet> affects_cache;
	unordered_map<StmtRef, StmtInfoPtrSet> affected_cache;
	unordered_map<StmtRef, StmtInfoPtrSet> affects_star_cache;
	unordered_map<StmtRef, StmtInfoPtrSet> affected_star_cache;

	pair<StmtRef, StmtRef> computeAllAffects(size_t graph_index);
	void transposeAffects(StmtRef start, StmtRef end);
	StmtInfoPtrSet buildAffectsGraphReverseVisit(StmtRef index, StmtRefSet& visited);
	void buildAffectsGraphVisit(StmtRef index, StmtRefSet& visited, stack<StmtRef>& stack);
	void buildAffectsGraph(size_t graph_index);

	StmtInfoPtrSet getAffectedByNodeAndVar(const shared_ptr<PKB::StatementNode>& node, VarRef variable);
	void processDFSVisit(Types::DFSInfo& info, StmtInfoPtrSet (*collector)(const shared_ptr<NodeInterface>&),
	                     void (AffectsManager::*processor)(Types::DFSInfo&, const shared_ptr<PKB::StatementNode>&));
	void processNodeAffects(Types::DFSInfo& info, const shared_ptr<PKB::StatementNode>& curr_stmt_node);
	void processNodeAffected(Types::DFSInfo& info, const shared_ptr<PKB::StatementNode>& curr_stmt_node);
	void processAffectStarBFS(Types::AffectStarBFSInfo& info, StmtInfoPtrSet (AffectsManager::*gatherer)(StmtRef),
	                          unordered_map<StmtRef, StmtInfoPtrSet>& cache);
	static void evaluateAffectStarBFSNode(const shared_ptr<StmtInfo>& stmt, Types::AffectsStarBFSInfo& info,
	                                      unordered_map<StmtRef, StmtInfoPtrSet>& cache);
};

#endif  // SPA_AFFECTSMANAGER_H

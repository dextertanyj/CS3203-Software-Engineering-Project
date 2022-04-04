#ifndef SPA_AFFECTSMANAGER_H
#define SPA_AFFECTSMANAGER_H

#include <unordered_map>

#include "PKB/CFG/ControlFlowGraph.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/InfoStore.h"
#include "PKB/ModifiesSRelation.h"
#include "PKB/NextManager.h"
#include "PKB/PKB.h"
#include "PKB/SVRelationStore.tpp"
#include "PKB/Types.h"
#include "PKB/UsesSRelation.h"

class PKB::AffectsManager {
public:
	explicit AffectsManager(ControlFlowGraph& control_flow_graph, SVRelationStore<PKB::ModifiesSRelation>& modifies_store,
	                        SVRelationStore<PKB::UsesSRelation>& uses_store);
	bool checkAffects(StmtRef first, StmtRef second);
	bool checkAffectsStar(StmtRef first, StmtRef second);
	StmtInfoPtrSet getAffects(StmtRef first);
	StmtInfoPtrSet getAffected(StmtRef second);
	StmtInfoPtrSet getAffectsStar(StmtRef node_ref);
	StmtInfoPtrSet getAffectedStar(StmtRef node_ref);
	void resetCache();

private:
	struct DFSInfo {
		VarRef variable;
		stack<shared_ptr<StmtInfo>> node_stack;
		StmtInfoPtrSet visited_set;
		StmtInfoPtrSet nodes;
	};

	struct CacheGraphNode {
		StmtInfoPtrSet statements;                           // NOLINT(misc-non-private-member-variables-in-classes)
		bool strongly_connected;                             // NOLINT(misc-non-private-member-variables-in-classes)
		unordered_set<shared_ptr<CacheGraphNode>> affected;  // NOLINT(misc-non-private-member-variables-in-classes)
		unordered_set<shared_ptr<CacheGraphNode>> affects;   // NOLINT(misc-non-private-member-variables-in-classes)

		CacheGraphNode(StmtInfoPtrSet statements, bool strongly_connected)
			: statements(move(statements)), strongly_connected(strongly_connected){};
	};

	ControlFlowGraph& control_flow_graph;
	SVRelationStore<PKB::UsesSRelation>& uses_store;
	SVRelationStore<PKB::ModifiesSRelation>& modifies_store;

	unordered_map<StmtRef, shared_ptr<CacheGraphNode>> cache_graph_store;
	unordered_map<StmtRef, StmtInfoPtrSet> affects_cache;
	unordered_map<StmtRef, StmtInfoPtrSet> affected_cache;
	unordered_map<StmtRef, StmtInfoPtrSet> affects_star_cache;
	unordered_map<StmtRef, StmtInfoPtrSet> affected_star_cache;

	StmtInfoPtrSet getAffectedLoop(StmtRef node, VarRef variable) const;
	void processDFSVisit(DFSInfo& info, void (AffectsManager::*processor)(DFSInfo&, const shared_ptr<StmtInfo>&) const) const;
	void processNodeAffects(DFSInfo& info, const shared_ptr<StmtInfo>& current) const;
	void processNodeAffected(DFSInfo& info, const shared_ptr<StmtInfo>& current) const;

	void buildCacheGraph(size_t graph_index);
	void computeAllAffects(StmtRef start, StmtRef end);
	void transposeAffects(StmtRef start, StmtRef end);
	void buildCacheGraphForwardVisit(StmtRef index, StmtRefSet& visited, stack<StmtRef>& stack) const;
	StmtInfoPtrSet buildCacheGraphReverseVisit(StmtRef index, StmtRefSet& visited) const;
	void processComponent(StmtRef index, StmtInfoPtrSet component);
	void connectRelevantComponents(StmtRef index);
};

#endif  // SPA_AFFECTSMANAGER_H

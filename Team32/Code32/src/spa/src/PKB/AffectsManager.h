#ifndef SPA_AFFECTSMANAGER_H
#define SPA_AFFECTSMANAGER_H

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
	explicit AffectsManager(ControlFlowGraph& control_flow_graph, const SVRelationStore<PKB::ModifiesSRelation>& modifies_store,
	                        const SVRelationStore<PKB::UsesSRelation>& uses_store);
	bool checkAffects(StmtRef first, StmtRef second);
	StmtInfoPtrSet getAffects(StmtRef first);
	StmtInfoPtrSet getAffected(StmtRef second);

private:
	ControlFlowGraph* control_flow_graph;
	SVRelationStore<PKB::UsesSRelation> uses_store;
	SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	StmtInfoPtrSet getAffectedByNodeAndVar(const shared_ptr<PKB::StatementNode>& node, VarRef variable);
	void processDFS(Types::DFSInfo& info, const shared_ptr<NodeInterface>& node,
	                StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface>&));
};

#endif  // SPA_AFFECTSMANAGER_H

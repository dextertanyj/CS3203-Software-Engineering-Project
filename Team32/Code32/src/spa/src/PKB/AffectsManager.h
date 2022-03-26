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
	explicit AffectsManager(ControlFlowGraph &control_flow_graph, SVRelationStore<PKB::ModifiesSRelation> &modifies_store,
	                        SVRelationStore<PKB::UsesSRelation> &uses_store);
	bool checkAffects(StmtRef first, StmtRef second);
	StmtInfoPtrSet getAffects(StmtRef first);
	StmtInfoPtrSet getAffected(StmtRef second);

private:
	ControlFlowGraph *control_flow_graph;
	SVRelationStore<PKB::UsesSRelation> *uses_store;
	SVRelationStore<PKB::ModifiesSRelation> *modifies_store;
	StmtInfoPtrSet getAffectedByNodeAndVar(const shared_ptr<PKB::StatementNode> &node, VarRef variable);
	void processDFSVisit(Types::DFSInfo &info, StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &),
	                     void (AffectsManager::*processor)(Types::DFSInfo &, const shared_ptr<PKB::StatementNode> &));
	void processNodeAffects(Types::DFSInfo &info, const shared_ptr<PKB::StatementNode> &curr_stmt_node);
	void processNodeAffected(Types::DFSInfo &info, const shared_ptr<PKB::StatementNode> &curr_stmt_node);
};

#endif  // SPA_AFFECTSMANAGER_H

#ifndef SPA_SRC_PKB_USESRELATION_H
#define SPA_SRC_PKB_USESRELATION_H

#include "PKB/CallStatementStore.h"
#include "PKB/PKB.h"
#include "PKB/ProcedureInfo.h"
#include "PKB/ProcedureStore.h"
#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/TopologicalSort.tpp"

class PKB::UsesRelation {
public:
	static bool validate(SVRelationStore<UsesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<UsesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variable);
	static void optimize(StatementRelationStore<ParentRelation>& parent_store, CallStatementStore& call_store, ProcedureStore& proc_store,
	                     TopologicalSort<ProcedureInfo>& topo_order, SVRelationStore<UsesRelation>& store);
private:
	static VarRefSet optimizeConditional(const shared_ptr<StmtInfo>& statement, StatementRelationStore<ParentRelation>& parent_store,
	                                     SVRelationStore<UsesRelation>& store);
	static VarRefSet optimizeCall(const shared_ptr<StmtInfo>& statement, CallStatementStore& call_store, ProcedureStore& proc_store,
	                              SVRelationStore<UsesRelation>& store);
};

#endif  // SPA_SRC_PKB_USESRELATION_H

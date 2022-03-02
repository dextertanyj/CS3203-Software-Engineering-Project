#ifndef SPA_SRC_PKB_USESRELATION_H
#define SPA_SRC_PKB_USESRELATION_H

#include "PKB/CallStatementStore.h"
#include "PKB/PKB.h"
#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/Types.h"
#include "PKB/TopologicalSort.h"

class PKB::UsesSRelation {
public:
public:
	static bool validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variable);
	static void optimize(StatementRelationStore<ParentRelation>& parent_store, CallStatementStore call_store, Types::ProcedureStore proc_store,
	                     TopologicalSort<ProcedureInfo> topo_order, SVRelationStore<UsesSRelation>& store);
	static VarRefSet optimizeConditional(shared_ptr<StmtInfo> statement, StatementRelationStore<ParentRelation>& parent_store,
	                                     SVRelationStore<UsesSRelation>& store);
	static VarRefSet optimizeCall(shared_ptr<StmtInfo> statement, CallStatementStore call_store, Types::ProcedureStore proc_store,
	                              SVRelationStore<UsesSRelation>& store);
};

#endif  // SPA_SRC_PKB_USESRELATION_H

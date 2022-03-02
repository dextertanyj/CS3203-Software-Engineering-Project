#ifndef SPA_SRC_PKB_USESRELATION_H
#define SPA_SRC_PKB_USESRELATION_H

#include "PKB/CallStatementStore.h"
#include "PKB/PKB.h"
#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/TopologicalSort.h"
#include "PKB/Types.h"

class PKB::UsesSRelation {
public:
	static bool validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variable);
	static void optimize(StatementRelationStore<ParentRelation>& parent_store, CallStatementStore& call_store,
	                     Types::ProcedureStore& proc_store, TopologicalSort<ProcedureInfo>& topo_order,
	                     SVRelationStore<UsesSRelation>& store);
	static VarRefSet optimizeCall(const shared_ptr<StmtInfo>& statement, CallStatementStore& call_store,
	                              Types::ProcedureStore& proc_store, SVRelationStore<UsesSRelation>& store);
	static VarRefSet optimizeConditional(const shared_ptr<StmtInfo>& statement,
	                                     StatementRelationStore<ParentRelation>& parent_store,
	                                     SVRelationStore<UsesRelation>& store);
};

#endif  // SPA_SRC_PKB_USESRELATION_H

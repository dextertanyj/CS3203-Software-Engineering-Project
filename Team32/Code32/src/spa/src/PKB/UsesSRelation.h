#ifndef SPA_SRC_PKB_USESRELATION_H
#define SPA_SRC_PKB_USESRELATION_H

#include "PKB/CallStatementStore.h"
#include "PKB/PKB.h"
#include "PKB/SVRelationStore.h"
#include "PKB/TopologicalSort.h"
#include "PKB/TransitiveRelationStore.tpp"
#include "PKB/Types.h"

class PKB::UsesSRelation {
public:
	static bool validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variable);
	static void optimize(Types::ParentStore& parent_store, CallStatementStore& call_store, Types::ProcedureStore& proc_store,
	                     TopologicalSort<ProcedureInfo>& topo_order, SVRelationStore<UsesSRelation>& store);

private:
	static void optimizeCall(const shared_ptr<StmtInfo>& statement, CallStatementStore& call_store, Types::ProcedureStore& proc_store,
	                         SVRelationStore<UsesSRelation>& store);
	static void optimizeConditional(const shared_ptr<StmtInfo>& statement, Types::ParentStore& parent_store,
	                                SVRelationStore<UsesSRelation>& store);
};

#endif  // SPA_SRC_PKB_USESRELATION_H

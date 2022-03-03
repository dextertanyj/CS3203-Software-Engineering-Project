#ifndef SPA_SRC_PKB_MODIFIESSRELATION_H
#define SPA_SRC_PKB_MODIFIESSRELATION_H

#include "PKB/CallStatementStore.h"
#include "PKB/PKB.h"
#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/TopologicalSort.h"
#include "PKB/Types.h"

class PKB::ModifiesSRelation {
public:
	static bool validate(SVRelationStore<ModifiesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<ModifiesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variables);
	static void optimize(StatementRelationStore<ParentRelation>& parent_store, CallStatementStore& call_store,
	                     Types::ProcedureStore& proc_store, TopologicalSort<ProcedureInfo>& topo_order,
	                     SVRelationStore<ModifiesSRelation>& store);

private:
	static VarRefSet optimizeCall(const shared_ptr<StmtInfo>& statement, CallStatementStore& call_store, Types::ProcedureStore& proc_store,
	                              SVRelationStore<ModifiesSRelation>& store);
	static VarRefSet optimizeConditional(const shared_ptr<StmtInfo>& statement, StatementRelationStore<ParentRelation>& parent_store,
	                                     SVRelationStore<ModifiesSRelation>& store);
};

#endif  // SPA_SRC_PKB_MODIFIESSRELATION_H

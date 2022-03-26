#ifndef SPA_SRC_PKB_MODIFIESSRELATION_H
#define SPA_SRC_PKB_MODIFIESSRELATION_H

#include "PKB/CallsStatementStore.h"
#include "PKB/PKB.h"
#include "PKB/SVRelationStore.h"
#include "PKB/TopologicalSort.h"
#include "PKB/TransitiveRelationStore.tpp"
#include "PKB/Types.h"

class PKB::ModifiesSRelation {
public:
	static bool validate(SVRelationStore<ModifiesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<ModifiesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variables);
	static void optimize(Types::ParentStore& parent_store, CallsStatementStore& call_store, Types::ProcedureStore& proc_store,
	                     TopologicalSort<ProcedureInfo>& topo_order, SVRelationStore<ModifiesSRelation>& store);

private:
	static void optimizeCall(const shared_ptr<StmtInfo>& statement, CallsStatementStore& call_store, Types::ProcedureStore& proc_store,
	                         SVRelationStore<ModifiesSRelation>& store);
	static void optimizeConditional(const shared_ptr<StmtInfo>& statement, Types::ParentStore& parent_store,
	                                SVRelationStore<ModifiesSRelation>& store);
	static void storeModifiedVars(shared_ptr<StmtInfo> stmt_key, StmtInfoPtrSet stmt_list, SVRelationStore<ModifiesSRelation>& store);
};

#endif  // SPA_SRC_PKB_MODIFIESSRELATION_H

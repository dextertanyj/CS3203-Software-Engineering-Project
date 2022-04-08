#ifndef SPA_SRC_PKB_MODIFIESSRELATION_H
#define SPA_SRC_PKB_MODIFIESSRELATION_H

#include "Common/TypeDefs.h"
#include "PKB/CallsStatementStore.h"
#include "PKB/PKB.h"
#include "PKB/ProcedureInfo.h"
#include "PKB/SVRelationStore.h"
#include "PKB/TopologicalSort.tpp"
#include "PKB/Types.h"

class PKB::ModifiesSRelation {
public:
	static bool validate(SVRelationStore<ModifiesSRelation>* store, const StmtInfoPtr& statement, const VarRef& variable);
	static bool validate(SVRelationStore<ModifiesSRelation>* store, const StmtInfoPtr& statement, const VarRefSet& variables);
	static void optimize(Types::ParentStore& parent_store, CallsStatementStore& call_store, Types::ProcedureStore& proc_store,
	                     TopologicalSort<ProcedureInfo>& topo_order, SVRelationStore<ModifiesSRelation>& store);

private:
	static void optimizeCall(const StmtInfoPtr& statement, CallsStatementStore& call_store, Types::ProcedureStore& proc_store,
	                         SVRelationStore<ModifiesSRelation>& store);
	static void optimizeConditional(const StmtInfoPtr& statement, Types::ParentStore& parent_store,
	                                SVRelationStore<ModifiesSRelation>& store);
	static void storeModifiedVars(const StmtInfoPtr& stmt_key, const StmtInfoPtrSet& stmt_list, SVRelationStore<ModifiesSRelation>& store);
};

#endif  // SPA_SRC_PKB_MODIFIESSRELATION_H

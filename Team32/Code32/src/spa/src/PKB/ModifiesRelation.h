#ifndef SPA_SRC_PKB_MODIFIESRELATION_H
#define SPA_SRC_PKB_MODIFIESRELATION_H

#include "PKB/CallStatementStore.h"
#include "PKB/PKB.h"
#include "PKB/ProcedureInfo.h"
#include "PKB/ProcedureStore.h"
#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/TopologicalSort.tpp"

class PKB::ModifiesRelation {
public:
	static bool validate(SVRelationStore<ModifiesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<ModifiesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variables);
	static void optimize(StatementRelationStore<ParentRelation>& parent_store, CallStatementStore& call_store, ProcedureStore& proc_store,
	                     TopologicalSort<ProcedureInfo>& topo_order, SVRelationStore<ModifiesRelation>& modifies_store);
private:
	static VarRefSet optimizeConditional(const shared_ptr<StmtInfo>& statement, StatementRelationStore<ParentRelation>& parent_store,
	                                     SVRelationStore<ModifiesRelation>& modifies_store);
	static VarRefSet optimizeCall(const shared_ptr<StmtInfo>& statement, CallStatementStore& call_store, ProcedureStore& proc_store,
	                              SVRelationStore<ModifiesRelation>& modifies_store);
};

#endif  // SPA_SRC_PKB_MODIFIESRELATION_H

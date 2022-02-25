#ifndef SPA_SRC_PKB_MODIFIESRELATION_H
#define SPA_SRC_PKB_MODIFIESRELATION_H

#include "PKB/PKB.h"
#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/StatementStore.h"

class PKB::ModifiesRelation {
public:
	static bool validate(SVRelationStore<ModifiesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<ModifiesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variables);
	static void optimize(StatementStore& statement_store, StatementRelationStore<ParentRelation>& parent_store,
	                     SVRelationStore<ModifiesRelation>& store);
};

#endif  // SPA_SRC_PKB_MODIFIESRELATION_H

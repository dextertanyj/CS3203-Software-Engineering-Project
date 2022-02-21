#ifndef SPA_MODIFES_H
#define SPA_MODIFES_H

#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/StatementStore.h"
#include "PKB/PKB.h"

class PKB::ModifiesRelation {
public:
	static bool validate(SVRelationStore<ModifiesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<ModifiesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variables);
	static void optimize(StatementStore& statement_store, StatementRelationStore<ParentRelation>& parent_store,
	                     SVRelationStore<ModifiesRelation>& store);
};

#endif  // SPA_MODIFES_H

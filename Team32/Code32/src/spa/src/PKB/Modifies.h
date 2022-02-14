#ifndef SPA_MODIFES_H
#define SPA_MODIFES_H

#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/StatementStore.h"

class Modifies {
public:
	static bool validate(SVRelationStore<Modifies>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static void optimize(StatementStore& statement_store, StatementRelationStore<ParentPKB>& parent_store,
	                     SVRelationStore<Modifies>& store);
};

#endif  // SPA_MODIFES_H

#ifndef SPA_MODIFES_H
#define SPA_MODIFES_H

#include "PKB/SVRelationStore.h"
#include "PKB/Types.h"
#include "PKB/StatementRelationStore.tpp"

class Modifies {
public:
	static bool validate(SVRelationStore<Modifies>* store, shared_ptr<StmtInfo> statement, VarRef variable);
	static void optimize(const StatementStore& statement_store, StatementRelationStore<ParentPKB>& parent_store, SVRelationStore<Modifies>& store);
};

#endif  // SPA_MODIFES_H

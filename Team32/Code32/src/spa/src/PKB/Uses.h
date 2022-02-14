#ifndef SPA_USES_H
#define SPA_USES_H

#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/StatementStore.h"

class Uses {
public:
	static bool validate(SVRelationStore<Uses>* store, shared_ptr<StmtInfo> statement, VarRef variable);
	static void optimize(StatementStore& statement_store, StatementRelationStore<ParentPKB>& parent_store, SVRelationStore<Uses>& store);
};

#endif  // SPA_USES_H

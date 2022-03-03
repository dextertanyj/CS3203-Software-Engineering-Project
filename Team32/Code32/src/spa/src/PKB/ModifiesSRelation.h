#ifndef SPA_SRC_PKB_MODIFIESRELATION_H
#define SPA_SRC_PKB_MODIFIESRELATION_H

#include "PKB/PKB.h"
#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/Types.h"

class PKB::ModifiesSRelation {
public:
	static bool validate(SVRelationStore<ModifiesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<ModifiesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variables);
	static void optimize(Types::StatementStore& statement_store, StatementRelationStore<ParentRelation>& parent_store,
	                     SVRelationStore<ModifiesSRelation>& store);
};

#endif  // SPA_SRC_PKB_MODIFIESRELATION_H

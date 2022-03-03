#ifndef SPA_SRC_PKB_USESRELATION_H
#define SPA_SRC_PKB_USESRELATION_H

#include "PKB/SVRelationStore.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/Types.h"

class PKB::UsesSRelation {
public:
	static bool validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable);
	static bool validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variable);
	static void optimize(Types::StatementStore& statement_store, StatementRelationStore<ParentRelation>& parent_store,
	                     SVRelationStore<UsesSRelation>& store);
};

#endif  // SPA_SRC_PKB_USESRELATION_H
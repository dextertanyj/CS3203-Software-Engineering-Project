#ifndef SPA_SRC_PKB_USESPRELATION_H
#define SPA_SRC_PKB_USESPRELATION_H

#include "PKB/PKB.h"
#include "PKB/PVRelationStore.tpp"
#include "PKB/SVRelationStore.tpp"
#include "PKB/Types.h"
#include "PKB/UsesSRelation.h"

class PKB::UsesPRelation {
public:
	static void optimize(const Types::ProcedureStore& proc_store, PVRelationStore<UsesPRelation>& store,
	                     SVRelationStore<UsesSRelation>& uses_s_store);
};
#endif  // SPA_SRC_PKB_USESPRELATION_H

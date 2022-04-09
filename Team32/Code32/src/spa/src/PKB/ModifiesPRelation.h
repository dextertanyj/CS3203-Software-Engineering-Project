#ifndef SPA_SRC_PKB_MODIFIESPRELATION_H
#define SPA_SRC_PKB_MODIFIESPRELATION_H

#include "PKB/ModifiesSRelation.h"
#include "PKB/PKB.h"
#include "PKB/PVRelationStore.tpp"
#include "PKB/SVRelationStore.tpp"
#include "PKB/Types.h"

class PKB::ModifiesPRelation {
public:
	static void optimize(const Types::ProcedureStore& proc_store, PVRelationStore<ModifiesPRelation>& store,
	                     SVRelationStore<ModifiesSRelation>& modifies_s_store);
};

#endif  // SPA_SRC_PKB_MODIFIESPRELATION_H

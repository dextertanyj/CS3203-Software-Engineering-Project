#ifndef SPA_SRC_PKB_MODIFIESPRELATION_H
#define SPA_SRC_PKB_MODIFIESPRELATION_H

#include "PKB/ModifiesSRelation.h"
#include "PKB/PKB.h"
#include "PKB/PVRelationStore.h"
#include "PKB/SVRelationStore.h"
#include "PKB/Types.h"

class PKB::ModifiesPRelation {
public:
	static void optimize(const Types::ProcedureStore&, PVRelationStore<ModifiesPRelation>&, SVRelationStore<ModifiesSRelation>&);
};

#endif  // SPA_SRC_PKB_MODIFIESPRELATION_H

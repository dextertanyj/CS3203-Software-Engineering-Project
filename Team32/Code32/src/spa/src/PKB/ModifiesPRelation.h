#ifndef SPA_MODIFIESPRELATION_H
#define SPA_MODIFIESPRELATION_H

#include "PKB/PKB.h"
#include "PKB/Storage.h"
#include "PKB/PVRelationStore.h"
#include "PKB/Types.h"

class PKB::ModifiesPRelation {
public:
	static void optimize(const Types::ProcedureStore&, PVRelationStore<ModifiesPRelation>&, SVRelationStore<ModifiesSRelation>&);
};

#endif //SPA_MODIFIESPRELATION_H

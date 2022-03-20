#ifndef SPA_SRC_PKB_USESPRELATION_H
#define SPA_SRC_PKB_USESPRELATION_H

#include "PKB/PKB.h"
#include "PKB/PVRelationStore.h"
#include "PKB/SVRelationStore.h"
#include "PKB/Storage.h"
#include "PKB/Types.h"

class PKB::UsesPRelation {
public:
	static void optimize(const Types::ProcedureStore&, PVRelationStore<UsesPRelation>&, SVRelationStore<UsesSRelation>&);
};
#endif  // SPA_SRC_PKB_USESPRELATION_H

#ifndef SPA_MODIFIESPRELATION_H
#define SPA_MODIFIESPRELATION_H

#include "PKB/PKB.h"
#include "PKB/Storage.h"
#include "PKB/PVRelationStore.h"

class PKB::ModifiesPRelation {
public:
    static void optimize(const TransitiveRelationStore<ProcRef , ProcedureInfo, CallRelation>&,
                         PVRelationStore<ModifiesPRelation>&, SVRelationStore<ModifiesSRelation>&, TopologicalSort<ProcedureInfo>&);
};

#endif //SPA_MODIFIESPRELATION_H

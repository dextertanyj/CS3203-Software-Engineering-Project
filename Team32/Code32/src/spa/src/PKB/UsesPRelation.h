#ifndef SPA_USESPRELATION_H
#define SPA_USESPRELATION_H

#include "PKB/PKB.h"
#include "PKB/Storage.h"
#include "PKB/PVRelationStore.h"
#include "PKB/TransitiveRelationStore.h"
#include "PKB/SVRelationStore.h"
#include "PKB/TopologicalSort.h"

class PKB::UsesPRelation {
public:
    static void optimize(const TransitiveRelationStore<ProcRef , ProcedureInfo, CallRelation>&,
                    PVRelationStore<UsesPRelation>&, SVRelationStore<UsesSRelation>&, TopologicalSort<ProcedureInfo>&);
};
#endif //SPA_USESPRELATION_H

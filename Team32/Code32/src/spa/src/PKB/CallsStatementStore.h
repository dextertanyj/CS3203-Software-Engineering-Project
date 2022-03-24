#ifndef SPA_SRC_PKB_CALLSSTATEMENTSTORE_H
#define SPA_SRC_PKB_CALLSSTATEMENTSTORE_H

#include <memory>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "PKB/CallsRelation.h"
#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.tpp"
#include "PKB/Types.h"

class PKB::CallsStatementStore {
public:
    CallsStatementStore();

    void set(const shared_ptr<StmtInfo> &statement, ProcRef procedure);

    [[nodiscard]] ProcRef getProcedure(const shared_ptr<StmtInfo> &statement) const;

    [[nodiscard]] ProcRef getProcedure(StmtRef index) const;

    void populate(const Types::ProcedureStore &procedures,
                  TransitiveRelationStore <ProcRef, ProcedureInfo, CallsRelation> &store) const;

private:
    unordered_map<StmtRef, ProcRef> map;
};

#endif  // SPA_SRC_PKB_CALLSSTATEMENTSTORE_H

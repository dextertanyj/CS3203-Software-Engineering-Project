#ifndef SPA_SRC_PKB_TYPES_H
#define SPA_SRC_PKB_TYPES_H

#include <memory>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/CallRelation.h"
#include "PKB/FollowsRelation.h"
#include "PKB/InfoStore.tpp"
#include "PKB/ParentRelation.h"
#include "PKB/ProcedureInfo.h"
#include "PKB/StatementInfo.h"

namespace PKB::Types {
typedef InfoStore<ProcRef, vector<shared_ptr<StmtInfo>>, ProcedureInfo> ProcedureStore;
typedef InfoStore<StmtRef, StmtType, StatementInfo> StatementStore;
typedef InfoStore<StmtRef, NodeInfo, NodeInfo> NodeStore;
typedef TransitiveRelationStore<ProcRef, ProcedureInfo, CallRelation> CallStore;
typedef TransitiveRelationStore<StmtRef, StmtInfo, ParentRelation> ParentStore;
typedef TransitiveRelationStore<StmtRef, StmtInfo, FollowsRelation> FollowsStore;
typedef InfoStore<StmtRef, StmtType, NodeInfo> NodeStore;
}  // namespace PKB::Types

#endif  // SPA_SRC_PKB_TYPES_H

#ifndef SPA_SRC_PKB_TYPES_H
#define SPA_SRC_PKB_TYPES_H

#include <memory>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/CallsRelation.h"
#include "PKB/FollowsRelation.h"
#include "PKB/InfoStore.tpp"
#include "PKB/ParentRelation.h"
#include "PKB/ProcedureInfo.h"
#include "PKB/StatementInfo.h"

namespace PKB::Types {
using ProcedureStore = InfoStore<ProcRef, vector<shared_ptr<StmtInfo>>, ProcedureInfo>;
using StatementStore = InfoStore<StmtRef, StmtType, StatementInfo>;
using CallStore = TransitiveRelationStore<ProcRef, ProcedureInfo, CallsRelation>;
using ParentStore = TransitiveRelationStore<StmtRef, StmtInfo, ParentRelation>;
using FollowsStore = TransitiveRelationStore<StmtRef, StmtInfo, FollowsRelation>;
enum class NodeType { Dummy, If, While, Statement };
}

#endif  // SPA_SRC_PKB_TYPES_H

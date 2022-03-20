#ifndef SPA_SRC_PKB_TYPES_H
#define SPA_SRC_PKB_TYPES_H

#include <memory>
#include <queue>
#include <unordered_set>
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
typedef TransitiveRelationStore<ProcRef, ProcedureInfo, CallRelation> CallStore;
typedef TransitiveRelationStore<StmtRef, StmtInfo, ParentRelation> ParentStore;
typedef TransitiveRelationStore<StmtRef, StmtInfo, FollowsRelation> FollowsStore;

typedef struct BFSInfo {
	const shared_ptr<PKB::NodeInterface>& start_node;
	queue<shared_ptr<PKB::NodeInterface>> node_queue;
	unordered_set<shared_ptr<PKB::NodeInterface>> visited_set;
	StmtInfoPtrSet nodes;
} BFSInfo;
}

#endif  // SPA_SRC_PKB_TYPES_H

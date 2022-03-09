#ifndef SPA_NEXTSTARQUERYCACHE_H
#define SPA_NEXTSTARQUERYCACHE_H

#include <set>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "NodeInfo.h"
#include "NodeRelation.h"
#include "PKB/PKB.h"

class NextStarQueryCache {
public:
	void initialize(PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>& cfg_store);
	bool checkNextStar(StmtRef first, StmtRef second);
	unordered_set<shared_ptr<PKB::NodeInfo>> getNextStar(StmtRef ref);

private:
	void cacheCheckNextStar(StmtRef first, StmtRef second);
	void cacheNextStarSet(StmtRef ref, unordered_set<shared_ptr<PKB::NodeInfo>> next_star_set);
	PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation> cfg_store;
	set<pair<StmtRef, StmtRef>> check_next_star_cache;
	unordered_map<StmtRef, unordered_set<shared_ptr<PKB::NodeInfo>>> next_star_set_cache;
};

#endif  // SPA_NEXTSTARQUERYCACHE_H

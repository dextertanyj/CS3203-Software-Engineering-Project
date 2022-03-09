#include "NextStarQueryCache.h"

#include "PKB/TransitiveRelationStore.tpp"

void NextStarQueryCache::initialize(PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation> cfg_store) {}
bool NextStarQueryCache::checkNextStar(StmtRef first, StmtRef second) { return false; }
unordered_set<shared_ptr<PKB::NodeInfo>> NextStarQueryCache::getNextStar(StmtRef ref) { return {}; }
void NextStarQueryCache::cacheCheckNextStar(StmtRef first, StmtRef second) {}
void NextStarQueryCache::cacheNextStarSet(StmtRef ref) {}

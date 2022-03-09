#include "NextStarQueryCache.h"

#include "PKB/TransitiveRelationStore.tpp"

void PKB::NextStarQueryCache::initialize(PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>& cfg) {
	this->cfg_store = cfg;
}

bool PKB::NextStarQueryCache::checkNextStar(StmtRef first, StmtRef second) {
	const bool is_in_cache = check_next_star_cache.find(make_pair(first, second)) != check_next_star_cache.end();
	if (!is_in_cache) {
		// Need to search CFG for the relationship.
		bool is_in_cfg = false;
		unordered_set<shared_ptr<PKB::NodeInfo>> next_star_set = searchNextStar(first);
		for (shared_ptr<PKB::NodeInfo> node_info : next_star_set) {
			if (node_info->getIdentifier() == second) is_in_cfg = true;
		}
		if (is_in_cfg) {
			cacheNextStarSet(first, next_star_set);
		}
		cacheCheckNextStar(first, second);
		return is_in_cfg;
	}
	return false;
}

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NextStarQueryCache::getNextStar(StmtRef ref) {
	const bool is_in_cache = next_star_set_cache.find(ref) != next_star_set_cache.end();
	if (!is_in_cache) {
		unordered_set<shared_ptr<PKB::NodeInfo>> next_star_set = searchNextStar(ref);
		cacheNextStarSet(ref, next_star_set);
		return next_star_set;
	}
	return next_star_set_cache.find(ref)->second;
}

void PKB::NextStarQueryCache::cacheCheckNextStar(StmtRef first, StmtRef second) {
	this->check_next_star_cache.insert(make_pair(first, second));
}

void PKB::NextStarQueryCache::cacheNextStarSet(StmtRef ref, unordered_set<shared_ptr<PKB::NodeInfo>> next_star_set) {
	this->next_star_set_cache.insert({ref, next_star_set});
}
unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NextStarQueryCache::searchNextStar(StmtRef ref) {
	// TODO: Search algorithm populates next* starting from ending node in the procedure.
	return {};
}

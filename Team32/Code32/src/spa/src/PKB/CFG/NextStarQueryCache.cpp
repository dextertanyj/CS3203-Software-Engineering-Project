#include "NextStarQueryCache.h"

#include "PKB/TransitiveRelationStore.tpp"

void PKB::NextStarQueryCache::initialize(PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>& cfg) {
	this->cfg_store = cfg;
}

bool PKB::NextStarQueryCache::checkNextStar(StmtRef first, StmtRef second) {
	const bool is_in_cache = check_next_star_cache.find(make_pair(first, second)) != check_next_star_cache.end();
	if (!is_in_cache) {
		bool is_in_cfg = false;
		unordered_set<shared_ptr<PKB::NodeInfo>> next_star_set = {};

		// Need to search CFG for the relationship.
		// TODO: Search logic

		if (is_in_cfg) {
			cacheCheckNextStar(first, second);
			cacheNextStarSet(first, next_star_set);
		}
		return is_in_cfg;
	}
	return false;
}

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NextStarQueryCache::getNextStar(StmtRef ref) {
	const bool is_in_cache = next_star_set_cache.find(ref) != next_star_set_cache.end();
	if (!is_in_cache) {
		bool is_in_cfg = false;
		unordered_set<shared_ptr<PKB::NodeInfo>> next_star_set = {};

		// Need to search CFG for the relationship.
		// TODO: Search logic

		if (is_in_cfg) {
			cacheNextStarSet(ref, next_star_set);
		}
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

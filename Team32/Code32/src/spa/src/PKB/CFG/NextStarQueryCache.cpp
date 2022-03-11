#include "NextStarQueryCache.h"

#include <queue>

#include "NodeComparator.h"
#include "PKB/TransitiveRelationStore.tpp"

void PKB::NextStarQueryCache::initialize(PKB::Types::NodeStore& node_info_store,
                                         PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>& cfg) {
	this->node_store = node_info_store;
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
	shared_ptr<NodeInfo> node = this->node_store.get(ref);
	size_t last_node_ref = node->getUniqueIndex();
	unordered_set<shared_ptr<PKB::NodeInfo>> next_nodes_transitive = {};
	std::priority_queue<pair<StmtRef, unordered_set<shared_ptr<PKB::NodeInfo>>>,
	                    vector<pair<StmtRef, unordered_set<shared_ptr<PKB::NodeInfo>>>>, NodeComparator>
		queue;
	queue.push(make_pair(last_node_ref, next_nodes_transitive));
	while (!queue.empty()) {
		StmtRef current_ref = queue.top().first;
		// TODO: Try not to make map public by using friend class in the future.
		NodeRelation current_node_relation = cfg_store.map.find(current_ref)->second;
		unordered_set<shared_ptr<NodeInfo>> nodes_to_add = queue.top().second;
		nodes_to_add = populateTransitive(current_node_relation, nodes_to_add);
		for (shared_ptr<PKB::NodeInfo> previous_node : current_node_relation.getForward()) {
			queue.push(make_pair(previous_node->getIdentifier(), nodes_to_add));
		}
		queue.pop();
	}
	return cfg_store.getReverseTransitive(ref);
}
unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NextStarQueryCache::populateTransitive(
	NodeRelation& current, unordered_set<shared_ptr<PKB::NodeInfo>> next_nodes_transitive) {
	// TODO: This will irreversibly add the next* set to the node. Which should not be done.
	current.appendReverseTransitive(next_nodes_transitive);
	next_nodes_transitive.insert(current.getSelf());
	return next_nodes_transitive;
}

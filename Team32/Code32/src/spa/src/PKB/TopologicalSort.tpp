#ifndef SPA_SRC_PKB_TOPOLOGICALSORT_TPP
#define SPA_SRC_PKB_TOPOLOGICALSORT_TPP

#include "PKB/TopologicalSort.h"

#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>

template <class TInfo>
PKB::TopologicalSort<TInfo>::TopologicalSort() = default;

template <class TInfo>
template <class TStore, typename TIdent, class TRelation>
void PKB::TopologicalSort<TInfo>::sort(const TStore& truth_store,
                                       const PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>& transitive_store) {
	order = vector<shared_ptr<TInfo>>();
	unordered_map<shared_ptr<TInfo>, pair<unordered_set<shared_ptr<TInfo>>, unordered_set<shared_ptr<TInfo>>>> edges;
	unordered_set<shared_ptr<TInfo>> all = truth_store.getAll();
	for (const shared_ptr<TInfo>& info : all) {
		TIdent ident = info->getIdentifier();
		unordered_set<shared_ptr<TInfo>> incoming_edges = transitive_store.getForward(ident);
		unordered_set<shared_ptr<TInfo>> outgoing_edges = transitive_store.getReverse(ident);
		edges.insert({info, {incoming_edges, outgoing_edges}});
	}
	// Topological sort using Kahn's Algorithm.
	queue<shared_ptr<TInfo>> next;
	for (auto key_value : edges) {
		if (key_value.second.first.empty()) {
			next.push(key_value.first);
		}
	}
	while (!next.empty()) {
		executeKahn(next, edges);
	}
	if (!edges.empty()) {
		throw invalid_argument("Recursive call detected.");
	}
}

template <class TInfo>
vector<shared_ptr<TInfo>> PKB::TopologicalSort<TInfo>::get() const {
	return order;
}

template <class TInfo>
void PKB::TopologicalSort<TInfo>::executeKahn(
	queue<shared_ptr<TInfo>>& next,
	unordered_map<shared_ptr<TInfo>, pair<unordered_set<shared_ptr<TInfo>>, unordered_set<shared_ptr<TInfo>>>>& edges) {
	shared_ptr<TInfo> info = next.front();
	next.pop();
	order.push_back(info);
	unordered_set<shared_ptr<TInfo>> outgoing = edges.at(info).second;
	for (shared_ptr<TInfo> node : outgoing) {
		edges.at(node).first.erase(info);
		if (edges.at(node).first.empty()) {
			next.push(node);
		}
	}
	edges.erase(info);
}

#endif  // SPA_SRC_PKB_TOPOLOGICALSORT_TPP

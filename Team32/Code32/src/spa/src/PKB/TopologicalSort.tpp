#ifndef SPA_SRC_PKB_TOPOLOGICALSORT_TPP
#define SPA_SRC_PKB_TOPOLOGICALSORT_TPP

#include "PKB/TopologicalSort.h"

template <class TInfo>
PKB::TopologicalSort<TInfo>::TopologicalSort() = default;

template <class TInfo>
template <class TStore, typename TIdent, class TRelation>
void PKB::TopologicalSort<TInfo>::sort(const TStore& truth_store,
                                       const PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>& transitive_store) {
	order = std::vector<TInfoPtr>();
	std::unordered_map<TInfoPtr, edge_set> edges;
	std::unordered_set<TInfoPtr> all = truth_store.getAll();
	for (const TInfoPtr& info : all) {
		TIdent ident = info->getIdentifier();
		std::unordered_set<TInfoPtr> incoming_edges = transitive_store.getForward(ident);
		std::unordered_set<TInfoPtr> outgoing_edges = transitive_store.getReverse(ident);
		edges.emplace(info, std::pair{incoming_edges, outgoing_edges});
	}
	// Topological sort using Kahn's Algorithm.
	std::queue<TInfoPtr> next;
	for (auto key_value : edges) {
		if (key_value.second.first.empty()) {
			next.push(key_value.first);
		}
	}
	while (!next.empty()) {
		executeKahn(next, edges);
	}
	if (!edges.empty()) {
		throw PKB::TopologicalSortException("Recursive call detected.");
	}
}

template <class TInfo>
std::vector<std::shared_ptr<TInfo>> PKB::TopologicalSort<TInfo>::get() const {
	return order;
}

template <class TInfo>
void PKB::TopologicalSort<TInfo>::executeKahn(std::queue<TInfoPtr>& next, std::unordered_map<TInfoPtr, edge_set>& edges) {
	TInfoPtr info = next.front();
	next.pop();
	order.push_back(info);
	std::unordered_set<TInfoPtr> outgoing = edges.at(info).second;
	for (TInfoPtr node : outgoing) {
		edges.at(node).first.erase(info);
		if (edges.at(node).first.empty()) {
			next.push(node);
		}
	}
	edges.erase(info);
}

#endif  // SPA_SRC_PKB_TOPOLOGICALSORT_TPP

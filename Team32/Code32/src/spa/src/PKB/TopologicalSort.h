#ifndef SPA_SRC_PKB_TOPOLOGICALSORT_H
#define SPA_SRC_PKB_TOPOLOGICALSORT_H

#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.tpp"

template <class TInfo>
class PKB::TopologicalSort {
public:
	TopologicalSort();
	template <class TStore, typename TIdent, class TRelation>
	void sort(const TStore& truth_store, const TransitiveRelationStore<TIdent, TInfo, TRelation>& transitive_store);
	[[nodiscard]] std::vector<std::shared_ptr<TInfo>> get() const;

private:
	using TInfoPtr = std::shared_ptr<TInfo>;
	using edge_set = std::pair<std::unordered_set<TInfoPtr>, std::unordered_set<TInfoPtr>>;

	std::vector<TInfoPtr> order;

	void executeKahn(std::queue<TInfoPtr>& next, std::unordered_map<TInfoPtr, edge_set>& edges);
};

#endif  // SPA_SRC_PKB_TOPOLOGICALSORT_H

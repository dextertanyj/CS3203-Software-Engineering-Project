#ifndef SPA_SRC_PKB_TOPOLOGICALSORT_H
#define SPA_SRC_PKB_TOPOLOGICALSORT_H

#include <memory>
#include <queue>
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

	void executeKahn(std::queue<TInfoPtr>& next, std::unordered_map<TInfoPtr, edge_set>& edges);
	std::vector<TInfoPtr> order;
};

#endif  // SPA_SRC_PKB_TOPOLOGICALSORT_H

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
	void executeKahn(
		std::queue<std::shared_ptr<TInfo>>& next,
		std::unordered_map<std::shared_ptr<TInfo>,
	                       std::pair<std::unordered_set<std::shared_ptr<TInfo>>, std::unordered_set<std::shared_ptr<TInfo>>>>& edges);
	std::vector<std::shared_ptr<TInfo>> order;
};

#endif  // SPA_SRC_PKB_TOPOLOGICALSORT_H

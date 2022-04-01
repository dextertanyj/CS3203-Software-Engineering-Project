#ifndef SPA_SRC_PKB_TOPOLOGICALSORT_H
#define SPA_SRC_PKB_TOPOLOGICALSORT_H

#include <memory>
#include <queue>
#include <vector>

#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.tpp"

using namespace std;

template <class TInfo>
class PKB::TopologicalSort {
public:
	TopologicalSort();
	template <class TStore, typename TIdent, class TRelation>
	void sort(const TStore& truth_store, const TransitiveRelationStore<TIdent, TInfo, TRelation>& transitive_store);
	[[nodiscard]] vector<shared_ptr<TInfo>> get() const;

private:
	void executeKahn(queue<shared_ptr<TInfo>>& next,
	                 unordered_map<shared_ptr<TInfo>, pair<unordered_set<shared_ptr<TInfo>>, unordered_set<shared_ptr<TInfo>>>>& edges);
	vector<shared_ptr<TInfo>> order;
};

#endif  // SPA_SRC_PKB_TOPOLOGICALSORT_H

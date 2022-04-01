#ifndef SPA_SRC_PKB_TOPOLOGICALSORT_H
#define SPA_SRC_PKB_TOPOLOGICALSORT_H

#include <memory>
#include <vector>

#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.tpp"

using namespace std;

template<class TInfo>
class PKB::TopologicalSort {
public:
    TopologicalSort();

    template<class TStore, typename TIdent, class TRelation>
    void sort(const TStore &truth_store, const TransitiveRelationStore <TIdent, TInfo, TRelation> &transitive_store);

    [[nodiscard]] vector<shared_ptr<TInfo>> get() const;

private:
    vector<shared_ptr<TInfo>> order;
};

#endif  // SPA_SRC_PKB_TOPOLOGICALSORT_H

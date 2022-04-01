#ifndef SPA_SRC_PKB_PARENTRELATION_H
#define SPA_SRC_PKB_PARENTRELATION_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/OrderedStatementTransitiveRelation.h"
#include "PKB/PKB.h"
#include "PKB/StatementInfo.h"
#include "PKB/TransitiveRelationStore.h"

using namespace std;

class PKB::ParentRelation : public PKB::OrderedStatementTransitiveRelation {
public:
    using OrderedStatementTransitiveRelation::OrderedStatementTransitiveRelation;

    void insertForward(const shared_ptr<StmtInfo> &parent_to_insert);

    void insertReverse(const shared_ptr<StmtInfo> &child_to_insert);

    [[nodiscard]] StmtInfoPtrSet getForward() const;

    [[nodiscard]] StmtInfoPtrSet getReverse() const;

private:
    shared_ptr<StmtInfo> parent;
    StmtInfoPtrSet children;
};

// Template specializations for Parent relationship.

template<>
void PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::ParentRelation>::optimize();

template<>
StmtInfoPtrSet
PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::ParentRelation>::populateTransitive(ParentRelation &current,
                                                                                         StmtInfoPtrSet previous);

#endif  // SPA_SRC_PKB_PARENTRELATION_H

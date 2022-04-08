#ifndef SPA_SRC_PKB_FOLLOWSRELATION_H
#define SPA_SRC_PKB_FOLLOWSRELATION_H

#include "Common/TypeDefs.h"
#include "PKB/OrderedStatementTransitiveRelation.h"
#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.tpp"

class PKB::FollowsRelation : public PKB::OrderedStatementTransitiveRelation {
public:
	using OrderedStatementTransitiveRelation::OrderedStatementTransitiveRelation;
	void insertForward(const StmtInfoPtr& following_to_insert);
	void insertReverse(const StmtInfoPtr& follower_to_insert);

	[[nodiscard]] StmtInfoPtrSet getForward() const;
	[[nodiscard]] StmtInfoPtrSet getReverse() const;

private:
	StmtInfoPtr following;
	StmtInfoPtr follower;
};

// Template specializations for Follows relationship.

template <>
void PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::FollowsRelation>::optimize();

template <>
StmtInfoPtrSet PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::FollowsRelation>::populateTransitive(FollowsRelation& current,
                                                                                                         StmtInfoPtrSet previous);

#endif  // SPA_SRC_PKB_FOLLOWSRELATION_H

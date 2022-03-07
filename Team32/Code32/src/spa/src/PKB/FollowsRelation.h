#ifndef SPA_SRC_PKB_FOLLOWSRELATION_H
#define SPA_SRC_PKB_FOLLOWSRELATION_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "PKB/StatementInfo.h"
#include "PKB/TransitiveRelationStore.h"

class PKB::FollowsRelation {
public:
	explicit FollowsRelation(shared_ptr<StmtInfo> self);
	void insertForward(const shared_ptr<StmtInfo>& following_to_insert);
	void insertReverse(const shared_ptr<StmtInfo>& follower_to_insert);
	void appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> followings);
	void appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> followers);

	[[nodiscard]] shared_ptr<StmtInfo> getSelf() const;
	[[nodiscard]] StmtInfoPtrSet getForward() const;
	[[nodiscard]] StmtInfoPtrSet getReverse() const;
	[[nodiscard]] StmtInfoPtrSet getForwardTransitive() const;
	[[nodiscard]] StmtInfoPtrSet getReverseTransitive() const;

private:
	shared_ptr<StmtInfo> self;
	shared_ptr<StmtInfo> following;
	shared_ptr<StmtInfo> follower;
	StmtInfoPtrSet following_transitive;
	StmtInfoPtrSet followers_transitive;
};

// Template specializations for Follows relationship.

template <>
void PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::FollowsRelation>::optimize();

template <>
StmtInfoPtrSet PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::FollowsRelation>::populateTransitive(FollowsRelation& current,
                                                                                                         StmtInfoPtrSet previous);

#endif  // SPA_SRC_PKB_FOLLOWSRELATION_H

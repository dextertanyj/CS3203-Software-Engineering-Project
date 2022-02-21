#ifndef SPA_FOLLOWSRELATION_H
#define SPA_FOLLOWSRELATION_H

#include <unordered_set>

#include "PKB/PKB.h"
#include "Common/TypeDefs.h"
#include "PKB/StatementRelationStore.h"

class PKB::FollowsRelation {
public:
	explicit FollowsRelation(shared_ptr<StmtInfo> self);
	void insertForward(shared_ptr<StmtInfo> following_to_insert);
	void insertReverse(shared_ptr<StmtInfo> follower_to_insert);
	void appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> followings);
	void appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> followers);

	StmtInfoPtrSet getForward();
	StmtInfoPtrSet getReverse();
	StmtInfoPtrSet getForwardTransitive();
	StmtInfoPtrSet getReverseTransitive();
	static void optimize(StatementRelationStore<FollowsRelation>& store);

private:
	shared_ptr<StmtInfo> self;
	shared_ptr<StmtInfo> following;
	shared_ptr<StmtInfo> follower;
	StmtInfoPtrSet following_transitive;
	StmtInfoPtrSet followers_transitive;
	static unordered_set<shared_ptr<StmtInfo>> populateTransitive(StatementRelationStore<FollowsRelation>& store, FollowsRelation& current,
	                                                              unordered_set<shared_ptr<StmtInfo>> previous);
};

#endif  // SPA_FOLLOWSRELATION_H

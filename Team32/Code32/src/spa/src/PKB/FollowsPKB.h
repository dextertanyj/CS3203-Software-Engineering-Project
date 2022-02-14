#ifndef SPA_FOLLOWSPKB_H
#define SPA_FOLLOWSPKB_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/StatementRelationStore.h"

class FollowsPKB {
public:
	explicit FollowsPKB(shared_ptr<StmtInfo> self);
	void insertForward(shared_ptr<StmtInfo> following);
	void insertReverse(shared_ptr<StmtInfo> follower);
	void appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> followers);
	void appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> followings);

	StmtInfoPtrSet getForward();
	StmtInfoPtrSet getReverse();
	StmtInfoPtrSet getForwardTransitive();
	StmtInfoPtrSet getReverseTransitive();
	static void optimize(StatementRelationStore<FollowsPKB>& store);

private:
	shared_ptr<StmtInfo> self;
	shared_ptr<StmtInfo> following;
	shared_ptr<StmtInfo> follower;
	StmtInfoPtrSet following_transitive;
	StmtInfoPtrSet followers_transitive;
	static unordered_set<shared_ptr<StmtInfo>> populateTransitive(StatementRelationStore<FollowsPKB>& store, FollowsPKB& current,
	                                                              unordered_set<shared_ptr<StmtInfo>> previous);
};

#endif  // SPA_FOLLOWSPKB_H

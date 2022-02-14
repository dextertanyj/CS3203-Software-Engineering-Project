#ifndef SPA_FOLLOWSPKB_H
#define SPA_FOLLOWSPKB_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/StatementRelationStore.h"

class FollowsPKB {
public:
	FollowsPKB(shared_ptr<StmtInfo> self);
	void insertForward(shared_ptr<StmtInfo> following);
	void insertReverse(shared_ptr<StmtInfo> follower);
	void appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> followers);
	void appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> followings);

	unordered_set<shared_ptr<StmtInfo>> getForward();
	unordered_set<shared_ptr<StmtInfo>> getReverse();
	unordered_set<shared_ptr<StmtInfo>> getForwardTransitive();
	unordered_set<shared_ptr<StmtInfo>> getReverseTransitive();
	static void optimize(StatementRelationStore<FollowsPKB>& store);

private:
	shared_ptr<StmtInfo> self;
	shared_ptr<StmtInfo> following;
	shared_ptr<StmtInfo> follower;
	unordered_set<shared_ptr<StmtInfo>> following_transitive;
	unordered_set<shared_ptr<StmtInfo>> followers_transitive;
	static unordered_set<shared_ptr<StmtInfo>> populateTransitive(StatementRelationStore<FollowsPKB>& store, FollowsPKB& current,
	                                                              unordered_set<shared_ptr<StmtInfo>> previous);
};

#endif  // SPA_FOLLOWSPKB_H

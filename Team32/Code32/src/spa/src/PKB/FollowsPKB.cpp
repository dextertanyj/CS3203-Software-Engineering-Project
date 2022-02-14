#include "FollowsPKB.h"
FollowsPKB::FollowsPKB(shared_ptr<StmtInfo> self) : self(self) {}

void FollowsPKB::insertForward(shared_ptr<StmtInfo> following) {
	if (this->following != nullptr) {
		throw "This statement is already following a statement";
	}
	this->following = following;
}

void FollowsPKB::insertReverse(shared_ptr<StmtInfo> follower) {
	if (this->follower != nullptr) {
		throw "This statement is already following a statement";
	}
	this->follower = follower;
}

void FollowsPKB::appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> followings) {
	this->following_transitive.insert(followings.begin(), followings.end());
}

void FollowsPKB::appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> followers) {
	this->followers_transitive.insert(followers.begin(), followers.end());
}

unordered_set<shared_ptr<StmtInfo>> FollowsPKB::getForward() {
	if (following == nullptr) {
		return {};
	}
	return {following};
}

unordered_set<shared_ptr<StmtInfo>> FollowsPKB::getReverse() {
	if (follower == nullptr) {
		return {};
	}
	return {follower};
}

unordered_set<shared_ptr<StmtInfo>> FollowsPKB::getForwardTransitive() { return following_transitive; }

unordered_set<shared_ptr<StmtInfo>> FollowsPKB::getReverseTransitive() { return followers_transitive; }

void FollowsPKB::optimize(StatementRelationStore<FollowsPKB>& store) {
	for (auto& item : store.map) {
		if (item.second.getForward().empty()) {
			FollowsPKB::populateTransitive(store, item.second, {});
		}
	}
}

unordered_set<shared_ptr<StmtInfo>> FollowsPKB::populateTransitive(StatementRelationStore<FollowsPKB>& store, FollowsPKB& current,
                                                                   unordered_set<shared_ptr<StmtInfo>> previous) {
	current.appendForwardTransitive(previous);
	previous.insert(current.self);
	unordered_set<shared_ptr<StmtInfo>> result;
	if (current.follower != nullptr) {
		auto follower = store.map.find(current.follower->reference);
		result = populateTransitive(store, follower->second, previous);
	}
	current.appendReverseTransitive(result);
	result.insert(current.self);
	return result;
}

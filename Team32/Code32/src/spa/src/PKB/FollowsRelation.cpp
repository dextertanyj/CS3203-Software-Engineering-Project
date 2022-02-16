#include "FollowsRelation.h"

#include <utility>
#include <iostream>

using namespace std;

FollowsRelation::FollowsRelation(shared_ptr<StmtInfo> self) : self(std::move(self)) {}

void FollowsRelation::insertForward(shared_ptr<StmtInfo> following) {
	if (self->reference <= following->reference) {
		throw invalid_argument("Statement out of order");
	}
	if (this->following != nullptr) {
		throw invalid_argument("This statement is already following a statement");
	}
	this->following = move(following);
}

void FollowsRelation::insertReverse(shared_ptr<StmtInfo> follower) {
	if (self->reference >= follower->reference) {
		throw invalid_argument("Statement out of order");
	}
	if (this->follower != nullptr) {
		throw invalid_argument("This statement is already following a statement");
	}
	this->follower = move(follower);
}

void FollowsRelation::appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> followings) {
	for (const auto& following : followings) {
		if (self->reference <= following->reference) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->following_transitive.insert(followings.begin(), followings.end());
}

void FollowsRelation::appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> followers) {
	for (const auto& follower : followers) {
		if (self->reference >= follower->reference) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->followers_transitive.insert(followers.begin(), followers.end());
}

StmtInfoPtrSet FollowsRelation::getForward() {
	if (following == nullptr) {
		return {};
	}
	return {following};
}

StmtInfoPtrSet FollowsRelation::getReverse() {
	if (follower == nullptr) {
		return {};
	}
	return {follower};
}

StmtInfoPtrSet FollowsRelation::getForwardTransitive() { return following_transitive; }

StmtInfoPtrSet FollowsRelation::getReverseTransitive() { return followers_transitive; }

void FollowsRelation::optimize(StatementRelationStore<FollowsRelation>& store) {
	for (auto& item : store.map) {
		if (item.second.getForward().empty()) {
			FollowsRelation::populateTransitive(store, item.second, {});
		}
	}
}

StmtInfoPtrSet FollowsRelation::populateTransitive(StatementRelationStore<FollowsRelation>& store, FollowsRelation& current,
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

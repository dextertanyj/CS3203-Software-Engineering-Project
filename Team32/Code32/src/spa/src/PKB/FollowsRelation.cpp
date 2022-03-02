#include "FollowsRelation.h"

#include <utility>

using namespace std;

PKB::FollowsRelation::FollowsRelation(shared_ptr<StmtInfo> self) : self(std::move(self)) {}

void PKB::FollowsRelation::insertForward(shared_ptr<StmtInfo> following_to_insert) {
	if (self->getIdentifier() <= following_to_insert->getIdentifier()) {
		throw invalid_argument("Statement out of order");
	}
	if (this->following != nullptr) {
		throw invalid_argument("This statement is already following_to_insert a statement");
	}
	this->following = move(following_to_insert);
}

void PKB::FollowsRelation::insertReverse(shared_ptr<StmtInfo> follower_to_insert) {
	if (self->getIdentifier() >= follower_to_insert->getIdentifier()) {
		throw invalid_argument("Statement out of order");
	}
	if (this->follower != nullptr) {
		throw invalid_argument("This statement already has a follower statement");
	}
	this->follower = move(follower_to_insert);
}

void PKB::FollowsRelation::appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> followings) {
	for (const auto& following_to_insert : followings) {
		if (self->getIdentifier() <= following_to_insert->getIdentifier()) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->following_transitive.insert(followings.begin(), followings.end());
}

void PKB::FollowsRelation::appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> followers) {
	for (const auto& follower_to_insert : followers) {
		if (self->getIdentifier() >= follower_to_insert->getIdentifier()) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->followers_transitive.insert(followers.begin(), followers.end());
}

StmtInfoPtrSet PKB::FollowsRelation::getForward() {
	if (following == nullptr) {
		return {};
	}
	return {following};
}

StmtInfoPtrSet PKB::FollowsRelation::getReverse() {
	if (follower == nullptr) {
		return {};
	}
	return {follower};
}

StmtInfoPtrSet PKB::FollowsRelation::getForwardTransitive() { return following_transitive; }

StmtInfoPtrSet PKB::FollowsRelation::getReverseTransitive() { return followers_transitive; }

void PKB::FollowsRelation::optimize(PKB::StatementRelationStore<PKB::FollowsRelation>& store) {
	for (auto& item : store.map) {
		if (item.second.getForward().empty()) {
			PKB::FollowsRelation::populateTransitive(store, item.second, {});
		}
	}
}

StmtInfoPtrSet PKB::FollowsRelation::populateTransitive(PKB::StatementRelationStore<PKB::FollowsRelation>& store,
                                                        PKB::FollowsRelation& current, unordered_set<shared_ptr<StmtInfo>> previous) {
	current.appendForwardTransitive(previous);
	previous.insert(current.self);
	unordered_set<shared_ptr<StmtInfo>> result;
	if (current.follower != nullptr) {
		auto follower = store.map.find(current.follower->getIdentifier());
		result = populateTransitive(store, follower->second, previous);
	}
	current.appendReverseTransitive(result);
	result.insert(current.self);
	return result;
}

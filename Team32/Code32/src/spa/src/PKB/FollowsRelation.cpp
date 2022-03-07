#include "FollowsRelation.h"

#include <utility>

using namespace std;

PKB::FollowsRelation::FollowsRelation(shared_ptr<StmtInfo> self) : self(std::move(self)) {}

void PKB::FollowsRelation::insertForward(const shared_ptr<StmtInfo>& following_to_insert) {
	if (self->getIdentifier() <= following_to_insert->getIdentifier()) {
		throw invalid_argument("Statement out of order");
	}
	if (this->following != nullptr) {
		throw invalid_argument("This statement is already following_to_insert a statement");
	}
	this->following = following_to_insert;
}

void PKB::FollowsRelation::insertReverse(const shared_ptr<StmtInfo>& follower_to_insert) {
	if (self->getIdentifier() >= follower_to_insert->getIdentifier()) {
		throw invalid_argument("Statement out of order");
	}
	if (this->follower != nullptr) {
		throw invalid_argument("This statement already has a follower statement");
	}
	this->follower = follower_to_insert;
}

void PKB::FollowsRelation::appendForwardTransitive(StmtInfoPtrSet followings) {
	for (const auto& following_to_insert : followings) {
		if (self->getIdentifier() <= following_to_insert->getIdentifier()) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->following_transitive.insert(followings.begin(), followings.end());
}

void PKB::FollowsRelation::appendReverseTransitive(StmtInfoPtrSet followers) {
	for (const auto& follower_to_insert : followers) {
		if (self->getIdentifier() >= follower_to_insert->getIdentifier()) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->followers_transitive.insert(followers.begin(), followers.end());
}

shared_ptr<StmtInfo> PKB::FollowsRelation::getSelf() const { return self; }

StmtInfoPtrSet PKB::FollowsRelation::getForward() const {
	if (following == nullptr) {
		return {};
	}
	return {following};
}

StmtInfoPtrSet PKB::FollowsRelation::getReverse() const {
	if (follower == nullptr) {
		return {};
	}
	return {follower};
}

StmtInfoPtrSet PKB::FollowsRelation::getForwardTransitive() const { return following_transitive; }

StmtInfoPtrSet PKB::FollowsRelation::getReverseTransitive() const { return followers_transitive; }

// Template specializations for Parent relationship.

template <>
void PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::FollowsRelation>::optimize() {
	for (auto& item : map) {
		if (item.second.getForward().empty()) {
			populateTransitive(item.second, {});
		}
	}
}

template <>
StmtInfoPtrSet PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::FollowsRelation>::populateTransitive(FollowsRelation& current,
                                                                                                         StmtInfoPtrSet previous) {
	current.appendForwardTransitive(previous);
	previous.insert(current.getSelf());
	StmtInfoPtrSet result;
	if (!current.getReverse().empty()) {
		auto follower = map.find((*current.getReverse().begin())->getIdentifier());
		result = populateTransitive(follower->second, previous);
	}
	current.appendReverseTransitive(result);
	result.insert(current.getSelf());
	return result;
}

#include "FollowsRelation.h"

void PKB::FollowsRelation::insertForward(const shared_ptr<StmtInfo>& following_to_insert) {
	if (getSelf()->getIdentifier() <= following_to_insert->getIdentifier()) {
		throw invalid_argument("Statement out of order");
	}
	if (this->following != nullptr) {
		throw invalid_argument("This statement is already following_to_insert a statement");
	}
	this->following = following_to_insert;
}

void PKB::FollowsRelation::insertReverse(const shared_ptr<StmtInfo>& follower_to_insert) {
	if (getSelf()->getIdentifier() >= follower_to_insert->getIdentifier()) {
		throw invalid_argument("Statement out of order");
	}
	if (this->follower != nullptr) {
		throw invalid_argument("This statement already has a follower statement");
	}
	this->follower = follower_to_insert;
}

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

// Template specializations for Follows relationship.

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
	previous.emplace(current.getSelf());
	StmtInfoPtrSet result;
	if (!current.getReverse().empty()) {
		auto follower = map.find((*current.getReverse().begin())->getIdentifier());
		result = populateTransitive(follower->second, previous);
	}
	current.appendReverseTransitive(result);
	result.emplace(current.getSelf());
	return result;
}

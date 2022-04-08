#include "PKB/FollowsRelation.h"

#include <cassert>
#include <unordered_map>

using namespace std;

void PKB::FollowsRelation::insertForward(const StmtInfoPtr& following_to_insert) {
	assert(getSelf()->getIdentifier() > following_to_insert->getIdentifier());
	assert(this->following == nullptr);

	this->following = following_to_insert;
}

void PKB::FollowsRelation::insertReverse(const StmtInfoPtr& follower_to_insert) {
	assert(getSelf()->getIdentifier() < follower_to_insert->getIdentifier());
	assert(this->follower == nullptr);

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

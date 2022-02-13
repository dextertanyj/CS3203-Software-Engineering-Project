#include "FollowStore.h"

#include <cassert>

using namespace std;

FollowStore::FollowStore() {}

// Also need to prevent same stmt No from following itself.
// need to add checks that stmtno 2 doesnt follow another other stmt and stmt no 1 isnt followed by other stmts
void FollowStore::setFollows(shared_ptr<StmtInfo> followed, shared_ptr<StmtInfo> follower) {
	StmtRef followed_idx = followed->reference;
	StmtRef follower_idx = follower->reference;

	if (followed_idx >= follower_idx) {
		throw invalid_argument("Second statement must come after the first statement");
	}

	auto followed_iter = followMap.find(followed_idx);
	if (followed_iter == followMap.end()) {
		// Parent does not exist in map
		FollowRelation followed_relation = {followed, nullptr, follower, {}, {}};
		followMap.insert(make_pair(followed_idx, followed_relation));
	} else if (followed_iter->second.follower == nullptr) {
		followed_iter->second.follower = follower;
	} else {
		throw invalid_argument("Statement already has a follower in the follow map");
	}

	auto following_iter = followMap.find(follower_idx);
	if (following_iter == followMap.end()) {
		FollowRelation follower_relation = {follower, followed, nullptr, {}, {}};
		followMap.insert(make_pair(follower_idx, follower_relation));
	} else if (following_iter->second.preceding == nullptr) {
		following_iter->second.preceding = followed;
	} else {
		throw invalid_argument("Statement already follows a statement");
	}
}

bool FollowStore::checkFollows(StmtRef followed, StmtRef following) {
	if (followed <= 0 || following <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	if (followed >= following) {
		return false;
	}

	auto iter = followMap.find(followed);
	if (iter == followMap.end()) {
		throw invalid_argument("Statement info 1 is not stored into the followMap.");
	}
	if (iter->second.follower == nullptr) {
		return false;
	}
	return iter->second.follower->reference == following;
}

shared_ptr<StmtInfo> FollowStore::getFollower(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto iter = followMap.find(index);
	if (iter == followMap.end()) {
		return nullptr;  // Statement not found in FollowStore.
	}
	return iter->second.follower;
}

shared_ptr<StmtInfo> FollowStore::getPreceding(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto iter = followMap.find(index);
	if (iter == followMap.end()) {
		return nullptr;
	}
	return iter->second.preceding;
}

unordered_set<shared_ptr<StmtInfo>> FollowStore::getFollowerStar(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto iter = followMap.find(index);
	if (iter == followMap.end()) {
		return unordered_set<shared_ptr<StmtInfo>>{};
	}
	return iter->second.followers;
}

unordered_set<shared_ptr<StmtInfo>> FollowStore::getPrecedingStar(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto iter = followMap.find(index);
	if (iter == followMap.end()) {
		return unordered_set<shared_ptr<StmtInfo>>{};
	}
	return iter->second.preceding_stmts;
}

void FollowStore::optimize() {
	for (auto& item : followMap) {
		if (item.second.preceding == nullptr) {
			populateTransitive(item.second, {});
		}
	}
}

unordered_set<shared_ptr<StmtInfo>> FollowStore::populateTransitive(FollowRelation& current, unordered_set<shared_ptr<StmtInfo>> previous) {
	current.preceding_stmts.insert(previous.begin(), previous.end());
	previous.insert(current.node);
	unordered_set<shared_ptr<StmtInfo>> result;
	if (current.follower != nullptr) {
		auto follower = followMap.find(current.follower->reference);
		result = populateTransitive(follower->second, previous);
	}
	current.followers.insert(result.begin(), result.end());
	result.insert(current.node);
	return result;
}

void FollowStore::clear() { followMap.clear(); }

#include "FollowStore.h"

#include <cassert>

using namespace std;

FollowStore::FollowStore() {}

// Also need to prevent same stmt No from following itself.
// need to add checks that stmtno 2 doesnt follow another other stmt and stmt no 1 isnt followed by other stmts
void FollowStore::setFollows(shared_ptr<StmtInfo> stmt_info_1, shared_ptr<StmtInfo> stmt_info_2) {
    StmtRef stmt_no_1 = stmt_info_1->reference;
    StmtRef stmt_no_2 = stmt_info_2->reference;

    if (stmt_no_1 >= stmt_no_2) throw invalid_argument("Second statement must come after the first statement.");
    if (stmt_no_1 <= 0 || stmt_no_2 <= 0) throw invalid_argument("Statement number must be a positive integer.");
    
    auto key_parent_stmt_itr = followMap.find(stmt_no_1);
    if (key_parent_stmt_itr == followMap.end()) {
        // Parent does not exist in map
        FollowRelation follow_relation = { nullptr, stmt_info_2, {}, {} };
        followMap.insert(make_pair(stmt_no_1, follow_relation));
    } else if (key_parent_stmt_itr->second.follower == nullptr) {
		key_parent_stmt_itr->second.follower = stmt_info_2;
    } else {
        throw invalid_argument("Statement 1 already has a follower in the follow map.");
    }
    auto key_child_stmt_itr = followMap.find(stmt_no_2);
    if (key_child_stmt_itr == followMap.end()) {
        FollowRelation follow_relation = {stmt_info_1, nullptr, {}, {} };
        followMap.insert(make_pair(stmt_no_2, follow_relation));
    } else if (key_child_stmt_itr->second.preceding == nullptr) {
		key_child_stmt_itr->second.preceding = stmt_info_1;
    } else {
        throw invalid_argument("Statement 2 already follows a statement.");
    }
}

bool FollowStore::checkFollows(shared_ptr<StmtInfo> stmtInfo1, shared_ptr<StmtInfo> stmtInfo2) {
    StmtRef stmtNo1 = stmtInfo1->reference;
    StmtRef stmtNo2 = stmtInfo2->reference;

    if (stmtNo1 <= 0 || stmtNo2 <= 0) throw invalid_argument("Statement number must be a positive integer.");
    if (stmtNo1 >= stmtNo2) return false;

    auto keyItr = followMap.find(stmtNo1);
    if (keyItr == followMap.end()) {
		throw invalid_argument("Statement info 1 is not stored into the followMap.");
    } else {
		return keyItr->second.follower == stmtInfo2;
    }
}

shared_ptr<StmtInfo> FollowStore::getFollower(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmt = stmtInfo->reference;
    if (stmt <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
    auto keyItr = followMap.find(stmt);
    if (keyItr == followMap.end()) {
		return nullptr; // Statement not found in FollowStore.
    }
	return keyItr->second.follower;
}

shared_ptr<StmtInfo> FollowStore::getPreceding(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmt = stmtInfo->reference;
    if (stmt <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
    auto keyItr = followMap.find(stmt);
    if (keyItr == followMap.end()) {
		return nullptr;
	}
	return keyItr->second.preceding;
}

void FollowStore::clear() {
    followMap.clear();
}

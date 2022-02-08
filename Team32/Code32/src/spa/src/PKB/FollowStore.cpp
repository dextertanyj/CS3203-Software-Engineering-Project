#include "FollowStore.h"

#include <cassert>

using namespace std;

FollowStore::FollowStore() {}

// Also need to prevent same stmt No from following itself.
// need to add checks that stmtno 2 doesnt follow another other stmt and stmt no 1 isnt followed by other stmts
void FollowStore::setFollows(shared_ptr<StmtInfo> stmtInfo1, shared_ptr<StmtInfo> stmtInfo2) {
    StmtRef stmtNo1 = (stmtInfo1.get())->reference;
    StmtRef stmtNo2 = (stmtInfo2.get())->reference;

    if (stmtNo1 >= stmtNo2) throw invalid_argument("Second statement must come after the first statement.");
    if (stmtNo1 <= 0 || stmtNo2 <= 0) throw invalid_argument("Statement number must be a positive integer.");
    
    auto keyItr = followMap.find(stmtNo1);
    if (keyItr == followMap.end()) {
        // does not exist in map
        FollowRelation followRelation = { stmtInfo2, {}, {} };
        followMap.insert(make_pair(stmtNo1, followRelation));
    } else {
        throw invalid_argument("Statement 1 already exists in follow map.");
    }
}

bool FollowStore::checkFollows(shared_ptr<StmtInfo> stmtInfo1, shared_ptr<StmtInfo> stmtInfo2) {
    StmtRef stmtNo1 = (stmtInfo1.get())->reference;
    StmtRef stmtNo2 = (stmtInfo2.get())->reference;

    if (stmtNo1 <= 0 || stmtNo2 <= 0) throw invalid_argument("Statement number must be a positive integer.");
    if (stmtNo1 >= stmtNo2) return false;

    auto keyItr = followMap.find(stmtNo1);
    if (keyItr != followMap.end()) {
        return keyItr->second.follower.get() == stmtInfo2.get();
    } else {
        return false;
    }
}

shared_ptr<StmtInfo> FollowStore::getFollower(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmt = (stmtInfo.get())->reference;

    if (stmt <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = followMap.find(stmt);
    if (keyItr != followMap.end()) {
        return keyItr->second.follower;
    } else {
        return make_shared<StmtInfo>();
    }
}

shared_ptr<StmtInfo> FollowStore::getFollowee(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmt = (stmtInfo.get())->reference;

    if (stmt <= 0) throw invalid_argument("Statement number must be a positive integer.");

    for(auto& itr : followMap) {
        if (itr.second.follower.get() == stmtInfo) {
            return itr.first;
        }
    }
    return -1;
}

void FollowStore::clear() {
    followMap.clear();
}

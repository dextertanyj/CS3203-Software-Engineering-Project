#include "FollowStore.h"

using namespace std;

FollowStore::FollowStore() {}

// Also need to prevent same stmt No from following itself.
// need to add checks that stmtno 2 doesnt follow another other stmt and stmt no 1 isnt followed by other stmts
void FollowStore::setFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    assert(stmtNo1 > 0 && stmtNo2 > 0);
    assert(stmtNo1 < stmtNo2);
    
    auto keyItr = followMap.find(stmtNo1);
    if (keyItr == followMap.end()) {
        // does not exist in map
        FollowRelation followRelation = { stmtNo2, {}, {} };
        followMap.insert(make_pair(stmtNo1, followRelation));
    } else {
        keyItr->second.follower = stmtNo2;
    }
}

bool FollowStore::checkFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    assert(stmtNo1 > 0 && stmtNo2 > 0);
    assert(stmtNo1 < stmtNo2);

    for(auto& itr : followMap) {
        if (itr.first == stmtNo1) {
            return itr.second.follower == stmtNo2;
        }
    }
    return false;
}

StmtRef FollowStore::getFollower(StmtRef stmt) {
    assert(stmt > 0);

    for(auto& itr : followMap) {
        if (itr.first == stmt) {
            return itr.second.follower;
        }
    }
    return -1;
}

StmtRef FollowStore::getFollowee(StmtRef stmt) {
    assert(stmt > 0);

    for(auto& itr : followMap) {
        if (itr.second.follower == stmt) {
            return itr.first;
        }
    }
    return -1;
}

void FollowStore::clear() {
    followMap.clear();
}

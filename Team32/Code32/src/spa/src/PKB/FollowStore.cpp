//
// Created by Yanzieee on 26/1/2022.
//

#include "FollowStore.h"
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

FollowStore::FollowStore() {}

// need to add checks that stmtno 2 doesnt follow another other stmt and stmt no 1 isnt followed by other stmts
void FollowStore::setFollows(int stmtNo1, int stmtNo2) {
    auto keyItr = followMap.find(stmtNo1);
    if (keyItr == followMap.end()) {
        // does not exist in map
        FollowRelation followRelation = { {stmtNo2}, {}, {} };
        followMap.insert(std::make_pair(stmtNo1, followRelation));
    } else {
        keyItr->second.follower = stmtNo2;
    }
}

bool FollowStore::checkFollows(int stmtNo1, int stmtNo2) {
    for(auto& itr : followMap) {
        if (itr.first == stmtNo1) {
            return itr.second.follower == stmtNo2;
        }
    }
    return false;
}

int FollowStore::getFollower(int stmt) {
    for(auto& itr : followMap) {
        if (itr.first == stmt) {
            return itr.second.follower;
        }
    }
    return -1;
}

int FollowStore::getFollowee(int stmt) {
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

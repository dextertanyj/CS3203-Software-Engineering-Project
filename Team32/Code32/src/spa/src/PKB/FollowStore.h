//
// Created by Yanzieee on 26/1/2022.
//

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_FOLLOWSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_FOLLOWSTORE_H

#endif

struct FollowRelation {
    int follower;
    unordered_set<int> followers;
    unordered_set<int> followees;
};

class FollowStore {
public:
    FollowStore();
    void setFollows(int stmtNo1, int stmtNo2);
    bool checkFollows(int stmtNo1, int stmtNo2);
    int getFollowee(int stmt);
    int getFollower(int stmt);
    void clear();
private:
    std::unordered_map<int, FollowRelation> followMap;
};

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../Common/TypeDefs.h"

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_FOLLOWSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_FOLLOWSTORE_H

struct FollowRelation {
    int follower;
    unordered_set<StmtRef> followers;
    unordered_set<StmtRef> followees;
};

class FollowStore {
public:
    FollowStore();
    void setFollows(StmtRef stmtNo1, StmtRef stmtNo2);
    bool checkFollows(StmtRef stmtNo1, StmtRef stmtNo2);
    StmtRef getFollowee(StmtRef stmt);
    StmtRef getFollower(StmtRef stmt);
    void clear();
private:
    unordered_map<StmtRef, FollowRelation> followMap;
};

#endif
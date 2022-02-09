#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "../Common/TypeDefs.h"

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_FOLLOWSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_FOLLOWSTORE_H

struct FollowRelation {
    shared_ptr<StmtInfo> followee;
    shared_ptr<StmtInfo> follower;
    unordered_set<shared_ptr<StmtInfo>> followers;
    unordered_set<shared_ptr<StmtInfo>> followees;
};

class FollowStore {
public:
    FollowStore();
    void setFollows(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    bool checkFollows(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    shared_ptr<StmtInfo> getFollowee(shared_ptr<StmtInfo>);
    shared_ptr<StmtInfo> getFollower(shared_ptr<StmtInfo>);
    void clear();
private:
    unordered_map<StmtRef, FollowRelation> followMap;
};

#endif
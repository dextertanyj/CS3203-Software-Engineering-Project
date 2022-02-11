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
    shared_ptr<StmtInfo> preceding;
    shared_ptr<StmtInfo> follower;
    unordered_set<shared_ptr<StmtInfo>> followers;
    unordered_set<shared_ptr<StmtInfo>> preceding_stmts;
};

class FollowStore {
public:
    FollowStore();
    void setFollows(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    bool checkFollows(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    shared_ptr<StmtInfo> getPreceding(shared_ptr<StmtInfo> stmtInfo);
    shared_ptr<StmtInfo> getFollower(shared_ptr<StmtInfo>);
    unordered_set<shared_ptr<StmtInfo>> getFollowerStar(shared_ptr<StmtInfo>);
    unordered_set<shared_ptr<StmtInfo>> getPrecedingStar(shared_ptr<StmtInfo>);
    void populateFollow(int numStatements);
    void clear();
private:
    void populateFollowers(int);
    void recursivelyAddFollowers(StmtRef, unordered_set<shared_ptr<StmtInfo>>&);
    void populatePrecedingStmts(int);
    void recursivelyAddPreceding(StmtRef, unordered_set<shared_ptr<StmtInfo>>&);
    unordered_map<StmtRef, FollowRelation> followMap;
};

#endif
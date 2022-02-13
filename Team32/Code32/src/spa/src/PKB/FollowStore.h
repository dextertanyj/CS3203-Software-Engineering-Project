#ifndef INC_21S2_CP_SPA_TEAM_32_FOLLOWSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_FOLLOWSTORE_H

#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../Common/TypeDefs.h"

using namespace std;

struct FollowRelation {
	shared_ptr<StmtInfo> node;
	shared_ptr<StmtInfo> preceding;
	shared_ptr<StmtInfo> follower;
	unordered_set<shared_ptr<StmtInfo>> followers;
	unordered_set<shared_ptr<StmtInfo>> preceding_stmts;
};

class FollowStore {
public:
	FollowStore();
	void setFollows(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
	bool checkFollows(StmtRef, StmtRef);
	shared_ptr<StmtInfo> getPreceding(StmtRef stmtInfo);
	shared_ptr<StmtInfo> getFollower(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getFollowerStar(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getPrecedingStar(StmtRef);
	void optimize();
	void clear();

private:
	unordered_set<shared_ptr<StmtInfo>> populateTransitive(FollowRelation& current, unordered_set<shared_ptr<StmtInfo>> previous);
	unordered_map<StmtRef, FollowRelation> followMap;
};

#endif
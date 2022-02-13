#pragma once

#include<stdio.h>
#include <iostream>
#include <memory>
#include <map>
#include <unordered_set>
#include "../Common/TypeDefs.h"

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H

struct ParentRelation {
	shared_ptr<StmtInfo> node;
	shared_ptr<StmtInfo> parent;
	unordered_set<shared_ptr<StmtInfo>> childSet;
	unordered_set<shared_ptr<StmtInfo>> parentStarSet;
	unordered_set<shared_ptr<StmtInfo>> childStarSet;
};

class ParentStore {
public:
    ParentStore();
    void setParent(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    bool isParentChild(StmtRef, StmtRef);
    
    // Get the immediate parent statement of this statement.
    shared_ptr<StmtInfo> getParent(StmtRef);
    // Get set of all child statements to stmt.
    unordered_set<shared_ptr<StmtInfo>> getChildren(StmtRef);
    unordered_set<shared_ptr<StmtInfo>> getParentStar(StmtRef);
    unordered_set<shared_ptr<StmtInfo>> getChildStar(StmtRef);
    void clear();
    void optimize();
private:
	unordered_set<shared_ptr<StmtInfo>> populateTransitive(ParentRelation& current, unordered_set<shared_ptr<StmtInfo>> previous);
	// key is parent stmtNo, value is a ParentRelation which contains information related to this statement.
	map<StmtRef, ParentRelation> parentMap;
};

#endif
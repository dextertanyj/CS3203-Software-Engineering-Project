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
    unordered_set<shared_ptr<StmtInfo>> childSet;
    unordered_set<shared_ptr<StmtInfo>> parentStarSet;
    unordered_set<shared_ptr<StmtInfo>> childStarSet;
    shared_ptr<StmtInfo> parent;
};

class ParentStore {
public:
    ParentStore();
    void setParent(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    bool isParentChild(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    
    // Get the immediate parent statement of this statement.
    shared_ptr<StmtInfo> getParent(shared_ptr<StmtInfo>);

    // Get set of all child statements to stmt.
    unordered_set<shared_ptr<StmtInfo>> getChildren(shared_ptr<StmtInfo>);

    unordered_set<shared_ptr<StmtInfo>> getParentStar(shared_ptr<StmtInfo>);
    unordered_set<shared_ptr<StmtInfo>> getChildStar(shared_ptr<StmtInfo>);
    void clear();
    void populateParent(int numStatements);
private:
    // key is parent stmtNo, value is a ParentRelation which contains information related to this statement.
    map<StmtRef, ParentRelation> parentMap;
    void populateParentStarSet(StmtRef StmtNo);
    void populateChildStarSet(StmtRef StmtNo);
    void recursivelyAddParent(StmtRef stmtNo, unordered_set<shared_ptr<StmtInfo>>&);
    void recursivelyAddChild(StmtRef stmtNo, unordered_set<shared_ptr<StmtInfo>>&);
};

#endif
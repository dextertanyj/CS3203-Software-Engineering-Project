#pragma once

#include<stdio.h>
#include <iostream>
#include <map>
#include <unordered_set>

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H

struct ParentRelation {
    unordered_set<int> childSet;
    unordered_set<int> parentStarSet;
    unordered_set<int> childStarSet;
    int parent;
};

class ParentStore {
public:
    ParentStore();
    void setParent(int parentStmt, int childStmt);
    bool isParentChild(int parentStmt, int childStmt);
    int getParent(int stmt);
    unordered_set<int> getChildren(int stmt);
    void clear();
    void populateParentStar();
    void setNumStatements(int size);
private:
    // key is parent stmtNo, value is list of children
    map<int, ParentRelation> parentMap;
    void populateParentStarSet(int StmtNo);
    void recursivelyAddParent(int stmtNo, unordered_set<int>& parentStarSet);
    int numStatements;
};

#endif
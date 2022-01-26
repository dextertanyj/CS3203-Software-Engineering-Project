//
// Created by Yanzieee on 26/1/2022.
//

#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H

#endif

struct ParentRelation {
    std::unordered_set<int> childSet;
    std::unordered_set<int> parentStarSet;
    std::unordered_set<int> childStarSet;
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
    std::map<int, ParentRelation> parentMap;
    void populateParentStarSet(int StmtNo);
    void recursivelyAddParent(int stmtNo, unordered_set<int>& parentStarSet);
    int numStatements;
};
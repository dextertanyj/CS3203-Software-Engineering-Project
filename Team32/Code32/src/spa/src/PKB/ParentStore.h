//
// Created by Yanzieee on 26/1/2022.
//

#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H

#endif

struct ParentRelation {
    std::unordered_set<int> childSet;
    std::unordered_set<int> parentStarSet;
    std::unordered_set<int> childStarSet;
};

class ParentStore {
public:
    ParentStore();
    void setParent(int parentStmt, int childStmt);
    bool isParentChild(int parentStmt, int childStmt);
    int getParent(int stmt);
    unordered_set<int> getChildren(int stmt);
    void clear();
private:
    // key is parent stmtNo, value is list of children
    std::unordered_map<int, ParentRelation> parentMap;
};
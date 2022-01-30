#pragma once

#include<stdio.h>
#include <iostream>
#include <map>
#include <unordered_set>
#include "../Common/TypeDefs.h"

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_PARENTSTORE_H

typedef int StmtRef;
typedef int StmtRef;
struct ParentRelation {
    unordered_set<StmtRef> childSet;
    unordered_set<StmtRef> parentStarSet;
    unordered_set<StmtRef> childStarSet;
    StmtRef parent;
};

class ParentStore {
public:
    ParentStore();
    void setParent(StmtRef parentStmt, StmtRef childStmt);
    bool isParentChild(StmtRef parentStmt, StmtRef childStmt);
    StmtRef getParent(StmtRef stmt);
    unordered_set<StmtRef> getChildren(StmtRef stmt);
    void clear();
    void populateParentStar();
    void setNumStatements(int size);
private:
    // key is parent stmtNo, value is list of children
    map<StmtRef, ParentRelation> parentMap;
    void populateParentStarSet(StmtRef StmtNo);
    void recursivelyAddParent(StmtRef stmtNo, unordered_set<StmtRef>& parentStarSet);
    int numStatements;
};

#endif
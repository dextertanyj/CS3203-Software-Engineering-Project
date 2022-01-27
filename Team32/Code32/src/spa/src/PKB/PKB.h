#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "ParentStore.h"
#include "FollowStore.h"
#include "UseStore.h"
#include "ModifyStore.h"
#include "../Common/TypeDefs.h"

typedef int StmtRef;
typedef string VarRef;
typedef int StmtRef;
using namespace std;

class PKB {
public:
    PKB();

    // Set methods called by Source processor
    bool setFollows(StmtRef stmtNo1, StmtRef stmtNo2);
    bool setParent(StmtRef stmtNo1, StmtRef stmtNo2);
    void setProc(ProcRef proc_name, vector<StmtRef> idxList);
    void setStmtType(StmtRef stmtNo, StmtType type);
    void setUses(StmtRef stmtNo, VarRef var_name);
    void setModifies(StmtRef stmtNo, VarRef var_name);

    // Get methods called by PQL
    // Parent get methods
    StmtRef getParent(StmtRef stmtNo);
    unordered_set<StmtRef> getChildren(StmtRef stmtNo);
    bool checkParents(StmtRef stmtNo1, StmtRef stmtNo2);

    // Follow get methods
    StmtRef getFollowee(StmtRef stmtNo);
    StmtRef getFollower(StmtRef stmtNo);
    bool checkFollows(StmtRef stmtNo1, StmtRef stmtNo2);

    // Others
    void clear();
    void setNumStatements(int size);
    void populateComplexRelations();

private:
    static ParentStore parentStore;
    static FollowStore followStore;
    static UseStore useStore;
    static ModifyStore modifyStore;
    bool checkInvalidStmts(StmtRef stmtNo1, StmtRef stmtNo2);
    static unordered_map<ProcRef, vector<StmtRef>> procMap;
    static unordered_map<StmtRef, StmtType> typeMap;
};

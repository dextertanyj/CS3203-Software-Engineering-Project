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
#include "AssignStore.h"
#include "../Common/TypeDefs.h"

using namespace std;

class PKB {
public:
    PKB();

    // Set methods called by Source processor
    void setFollows(shared_ptr<StmtInfo> stmtNo1, shared_ptr<StmtInfo> stmtNo2);
    void setParent(shared_ptr<StmtInfo> stmtNo1, shared_ptr<StmtInfo> stmtNo2);
    void setProc(ProcRef proc_name, vector<StmtRef> idxList);
    void setStmtType(StmtRef stmtNo, StmtType type);
    void setUses(shared_ptr<StmtInfo> stmtNo, VarRef var_name);
    void setModifies(shared_ptr<StmtInfo> stmtNo, VarRef var_name);
    void setAssign(StmtRef stmtNo, VarRef variableLHS, string opTree);

    // Get methods called by PQL
    // General get methods
    StmtRefList getStatements();

    // Parent get methods
    shared_ptr<StmtInfo> getParent(shared_ptr<StmtInfo> stmtNo);
    unordered_set<shared_ptr<StmtInfo>> getChildren(shared_ptr<StmtInfo> stmtNo);
    bool checkParents(shared_ptr<StmtInfo> stmtNo1, shared_ptr<StmtInfo> stmtNo2);

    // Follow get methods
    shared_ptr<StmtInfo> getFollowee(shared_ptr<StmtInfo> stmtNo);
    shared_ptr<StmtInfo> getFollower(shared_ptr<StmtInfo> stmtNo);
    bool checkFollows(shared_ptr<StmtInfo> stmtNo1, shared_ptr<StmtInfo> stmtNo2);

    // Others
    void clear();
    void setNumStatements(int size);
    void populateComplexRelations();

private:
    ParentStore parentStore;
    FollowStore followStore;
    UseStore useStore;
    ModifyStore modifyStore;
    AssignStore assignStore;
    unordered_map<ProcRef, vector<StmtRef>> procMap;
    unordered_map<StmtRef, StmtType> typeMap;
    int numStatements;
    void checkInvalidStmt(StmtRef stmtNo1);
};

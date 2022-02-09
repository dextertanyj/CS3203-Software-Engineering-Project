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
#include "ProcStore.h"
#include "../Common/TypeDefs.h"

using namespace std;

class PKB {
public:
    PKB();

    // Set methods called by Source processor
    void setFollows(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    void setParent(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
    void setProc(ProcRef, vector<shared_ptr<StmtInfo>>);
    void setStmtType(StmtRef, StmtType);
    void setUses(shared_ptr<StmtInfo>, VarRef);
    void setModifies(shared_ptr<StmtInfo>, VarRef);
    void setAssign(StmtRef, VarRef variableLHS, string opTree);
    void setCall(shared_ptr<StmtInfo>, ProcRef);

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

    // Use get methods
    bool checkUses(shared_ptr<StmtInfo>, VarRef);
    bool checkProcUses(ProcRef, VarRef);
    unordered_set<shared_ptr<StmtInfo>> getUsesByVar(VarRef);
    unordered_set<ProcRef> getProcUsesByVar(VarRef);
    unordered_set<VarRef> getUsesByStmt(shared_ptr<StmtInfo>);
    unordered_set<VarRef> getUsesByProc(ProcRef);

    // Others
    void clear();
    ProcRef getProcFromCall(shared_ptr<StmtInfo>);
    void setNumStatements(int size);
    void populateComplexRelations();

private:
    ParentStore parentStore;
    FollowStore followStore;
    UseStore useStore;
    ModifyStore modifyStore;
    AssignStore assignStore;
    ProcStore procStore;
    unordered_map<StmtRef, StmtType> typeMap;
    int numStatements;
    void checkInvalidStmt(StmtRef stmtNo1);
};

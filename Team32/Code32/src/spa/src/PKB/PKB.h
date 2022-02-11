#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ParentStore.h"
#include "FollowStore.h"
#include "UseStore.h"
#include "ModifyStore.h"
#include "AssignStore.h"
#include "ProcStore.h"
#include "Common/TypeDefs.h"
#include "Common/ArithmeticProcessor/ArithmeticExpression.h"

using namespace std;

class PKB {
public:
    PKB();

    // Set methods called by Source processor
    void setFollows(StmtRef, StmtRef);
    void setParent(StmtRef, StmtRef);
    void setProc(ProcRef, StmtRefList);
    void setStmtType(StmtRef, StmtType);
    void setUses(StmtRef, VarRef);
    void setModifies(StmtRef, VarRef);
    void setAssign(StmtRef, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree);
    void setCall(StmtRef, ProcRef);

    // Get methods called by PQL
    // General get methods
    StmtInfoList getStatements();

    // Parent get methods
    shared_ptr<StmtInfo> getParent(StmtRef);
    unordered_set<shared_ptr<StmtInfo>> getChildren(StmtRef);
    bool checkParents(StmtRef, StmtRef);
    unordered_set<shared_ptr<StmtInfo>> getParentStar(StmtRef);
    unordered_set<shared_ptr<StmtInfo>> getChildStar(StmtRef);

    // Follow get methods
    shared_ptr<StmtInfo> getPreceding(StmtRef stmt);
    shared_ptr<StmtInfo> getFollower(StmtRef);
    bool checkFollows(StmtRef, StmtRef);
    unordered_set<shared_ptr<StmtInfo>> getFollowerStar(StmtRef);
    unordered_set<shared_ptr<StmtInfo>> getPrecedingStar(StmtRef);

    // Use get methods
    bool checkUses(StmtRef, VarRef);
    bool checkProcUses(ProcRef, VarRef);
    unordered_set<shared_ptr<StmtInfo>> getUsesByVar(VarRef);
    unordered_set<ProcRef> getProcUsesByVar(VarRef);
    unordered_set<VarRef> getUsesByStmt(StmtRef);
    unordered_set<VarRef> getUsesByProc(ProcRef);

    // Modify get methods
    bool checkModifies(StmtRef, VarRef);
    bool checkProcModifies(ProcRef, VarRef);
    unordered_set<shared_ptr<StmtInfo>> getModifiesByVar(VarRef);
    unordered_set<ProcRef> getProcModifiesByVar(VarRef);
    unordered_set<VarRef> getModifiesByStmt(StmtRef);
    unordered_set<VarRef> getModifiesByProc(ProcRef);

    // Assign get methods
    bool isPattern(VarRef, Common::ArithmeticProcessor::ArithmeticExpression, bool isRHSExactMatchNeeded);
    StmtInfoList getPatternMatch(StmtInfoList, VarRef, Common::ArithmeticProcessor::ArithmeticExpression, bool isRHSExactMatchNeeded);
    StmtInfoList getAllPatternMatch(VarRef, Common::ArithmeticProcessor::ArithmeticExpression, bool isRHSExactMatchNeeded);
    StmtInfoList getPatternMatchLHS(VarRef);
    vector<pair<shared_ptr<StmtInfo>, VarRef>> getPatternMatchRHS(Common::ArithmeticProcessor::ArithmeticExpression, bool isRHSExactMatchNeeded);

    // Others
    void clear();
    ProcRef getProcFromCall(StmtRef);
    void populateComplexRelations();
	unordered_map<StmtRef, shared_ptr<StmtInfo>> getStmtInfoMap();
	int getNumStatements();
private:
    ParentStore parentStore;
    FollowStore followStore;
    UseStore useStore;
    ModifyStore modifyStore;
    AssignStore assignStore;
    ProcStore procStore;
    void setNumStatements();
    unordered_map<StmtRef, shared_ptr<StmtInfo>> stmtInfoMap; // Stores StmtInfo for a particular StmtRef.
    int numStatements;
};

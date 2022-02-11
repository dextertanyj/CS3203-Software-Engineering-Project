#pragma once

#include<stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "memory"
#include "iostream"
#include "../Common/TypeDefs.h"

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H

class ModifyStore {
public:
    ModifyStore();
    void setModify(shared_ptr<StmtInfo>, VarRef);
    bool checkModifies(shared_ptr<StmtInfo>, VarRef);
    bool checkModifiesList(vector<shared_ptr<StmtInfo>>, VarRef);
    unordered_set<shared_ptr<StmtInfo>> getModifiesByVar(VarRef);
    VarRef getModifiesByStmt(shared_ptr<StmtInfo>);
    unordered_set<VarRef> getModifiesByStmtList(vector<shared_ptr<StmtInfo>>);
    void clear();
private:
    unordered_map<StmtRef, VarRef> stmtToVarMap;
    unordered_map<VarRef, unordered_set<shared_ptr<StmtInfo>>> varToStmtMap;
};

#endif //INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H

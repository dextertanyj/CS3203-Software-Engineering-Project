#pragma once

#include<stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../Common/TypeDefs.h"

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_USESTORE_H
#define INC_21S2_CP_SPA_TEAM_32_USESTORE_H

class UseStore {
public:
    UseStore();
    void setUses(shared_ptr<StmtInfo>, VarRef);
    bool checkUses(shared_ptr<StmtInfo>, VarRef);
    bool checkUsesList(vector<shared_ptr<StmtInfo>>, VarRef);
    unordered_set<shared_ptr<StmtInfo>> getUsesByVar(VarRef);
    unordered_set<VarRef> getUsesByStmt(shared_ptr<StmtInfo>);
    unordered_set<VarRef> getUsesByStmtList(vector<shared_ptr<StmtInfo>>);

    void clear();
private:
    unordered_map<StmtRef, unordered_set<VarRef>> stmtToVarMap;
    unordered_map<VarRef, unordered_set<shared_ptr<StmtInfo>>> varToStmtMap;
};

#endif //INC_21S2_CP_SPA_TEAM_32_USESTORE_H

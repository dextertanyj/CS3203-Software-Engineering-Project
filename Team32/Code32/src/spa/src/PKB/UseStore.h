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
    void setUses(StmtRef stmtNo, VarRef var_name);
    void clear();
private:
    unordered_map<StmtRef, unordered_set<VarRef>> stmtToVarMap;
    unordered_map<VarRef, unordered_set<StmtRef>> varToStmtMap;
};

#endif //INC_21S2_CP_SPA_TEAM_32_USESTORE_H

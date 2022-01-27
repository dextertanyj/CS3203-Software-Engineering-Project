#pragma once

#include<stdio.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#ifndef INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H

class ModifyStore {
public:
    ModifyStore();
    void setModify(int stmtNo, string var_name);
private:
    unordered_map<int, unordered_set<string>> stmtToVarMap;
    unordered_map<string, unordered_set<int>> varToStmtMap;
};

#endif //INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H

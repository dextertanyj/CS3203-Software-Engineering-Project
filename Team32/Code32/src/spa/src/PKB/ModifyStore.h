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

#ifndef INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H

#endif //INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H

class ModifyStore {
public:
    ModifyStore();
    void setModify(int stmtNo, std::string var_name);
private:
    std::unordered_map<int, unordered_set<std::string>> stmtToVarMap;
    std::unordered_map<std::string, unordered_set<int>> varToStmtMap;
};

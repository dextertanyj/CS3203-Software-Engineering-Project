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

#ifndef INC_21S2_CP_SPA_TEAM_32_USESTORE_H
#define INC_21S2_CP_SPA_TEAM_32_USESTORE_H

#endif //INC_21S2_CP_SPA_TEAM_32_USESTORE_H

class UseStore {
public:
    UseStore();
    void setUses(int stmtNo, std::string var_name);
private:
    std::unordered_map<int, unordered_set<std::string>> stmtToVarMap;
    std::unordered_map<std::string, unordered_set<int>> varToStmtMap;
};

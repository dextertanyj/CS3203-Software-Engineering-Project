//
// Created by Yanzieee on 26/1/2022.
//

#include "UseStore.h"
#include <unordered_map>

using namespace std;

UseStore::UseStore() {}

void UseStore::setUses(int stmtNo, std::string var_name) {
    auto keyItr = varToStmtMap.find(var_name);
    if (keyItr == varToStmtMap.end()) {
        varToStmtMap.insert(std::make_pair(var_name, unordered_set<int>{stmtNo}));
    } else {
        keyItr->second.insert(stmtNo);
    }
    auto keyItr2 = stmtToVarMap.find(stmtNo);
    if (keyItr2 == stmtToVarMap.end()) {
        stmtToVarMap.insert(std::make_pair(stmtNo, unordered_set<std::string>{var_name}));
    } else {
        keyItr2->second.insert(var_name);
    }
}

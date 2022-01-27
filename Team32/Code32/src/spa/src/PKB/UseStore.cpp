#include "UseStore.h"

using namespace std;

UseStore::UseStore() {}

void UseStore::setUses(int stmtNo, string var_name) {
    auto keyItr = varToStmtMap.find(var_name);
    if (keyItr == varToStmtMap.end()) {
        varToStmtMap.insert(make_pair(var_name, unordered_set<int>{stmtNo}));
    } else {
        keyItr->second.insert(stmtNo);
    }
    auto keyItr2 = stmtToVarMap.find(stmtNo);
    if (keyItr2 == stmtToVarMap.end()) {
        stmtToVarMap.insert(make_pair(stmtNo, unordered_set<string>{var_name}));
    } else {
        keyItr2->second.insert(var_name);
    }
}

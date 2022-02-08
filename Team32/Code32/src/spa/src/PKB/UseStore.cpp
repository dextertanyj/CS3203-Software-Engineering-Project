#include "UseStore.h"

#include <cassert>

using namespace std;

UseStore::UseStore() {}

void UseStore::setUses(StmtRef stmtNo, VarRef var_name) {
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = varToStmtMap.find(var_name);
    if (keyItr == varToStmtMap.end()) {
        varToStmtMap.insert(make_pair(var_name, unordered_set<StmtRef>{stmtNo}));
    } else {
        keyItr->second.insert(stmtNo);
    }
    auto keyItr2 = stmtToVarMap.find(stmtNo);
    if (keyItr2 == stmtToVarMap.end()) {
        stmtToVarMap.insert(make_pair(stmtNo, unordered_set<VarRef>{var_name}));
    } else {
        keyItr2->second.insert(var_name);
    }
}

void UseStore::clear() {
    varToStmtMap.clear();
    stmtToVarMap.clear();
}

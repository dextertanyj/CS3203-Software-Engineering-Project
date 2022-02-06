#include "ModifyStore.h"

#include <cassert>

using namespace std;

ModifyStore::ModifyStore() {}

void ModifyStore::setModify(StmtRef stmtNo, VarRef var_name) {
    assert(stmtNo > 0);

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

void ModifyStore::clear() {
    varToStmtMap.clear();
    stmtToVarMap.clear();
}


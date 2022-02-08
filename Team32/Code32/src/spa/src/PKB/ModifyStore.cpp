#include "ModifyStore.h"

#include <cassert>

using namespace std;

ModifyStore::ModifyStore() {}

void ModifyStore::setModify(shared_ptr<StmtInfo> stmtInfo, VarRef var_name) {
    StmtRef stmtNo = (stmtInfo.get())->reference;

    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = varToStmtMap.find(var_name);
    if (keyItr == varToStmtMap.end()) {
        varToStmtMap.insert(make_pair(var_name, unordered_set<shared_ptr<StmtInfo>>{stmtInfo}));
    } else {
        keyItr->second.insert(stmtInfo);
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


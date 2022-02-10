#include "ModifyStore.h"

#include <cassert>

using namespace std;

ModifyStore::ModifyStore() {}

void ModifyStore::setModify(shared_ptr<StmtInfo> stmtInfo, VarRef var_name) {
    StmtRef stmtNo = stmtInfo->reference;

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

bool ModifyStore::checkModifies(shared_ptr<StmtInfo> stmtInfo, VarRef varName) {
    StmtRef stmtNo = stmtInfo->reference;
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = varToStmtMap.find(varName);
    if (keyItr == varToStmtMap.end()) return false;

    for (auto& itr : keyItr->second) {
        if (stmtInfo == itr) {
            return true;
        }
    }
    return false;
}

bool ModifyStore::checkModifiesList(vector<shared_ptr<StmtInfo>> stmtInfoList, VarRef varName) {
    for (auto& itr : stmtInfoList) {
        if (itr->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");
        if (checkModifies(itr, varName)) return true;
    }
    return false;
}

unordered_set<VarRef> ModifyStore::getModifiesByStmt(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmtNo = stmtInfo->reference;
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = stmtToVarMap.find(stmtNo);
    if (keyItr == stmtToVarMap.end()) {
        return unordered_set<VarRef>{};
    } else {
        return keyItr->second;
    }
}

unordered_set<shared_ptr<StmtInfo>> ModifyStore::getModifiesByVar(VarRef varName) {
    auto keyItr = varToStmtMap.find(varName);
    if (keyItr == varToStmtMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>{};
    } else {
        return keyItr->second;
    }
}

unordered_set<VarRef> ModifyStore::getModifiesByStmtList(vector<shared_ptr<StmtInfo>> stmtInfoList) {
    unordered_set<VarRef> varSet;
    for (auto& itr : stmtInfoList) {
        if (itr->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");

        unordered_set<VarRef> stmtVarSet = getModifiesByStmt(itr);
        varSet.insert(stmtVarSet.begin(), stmtVarSet.end());
    }
    return varSet;
}

void ModifyStore::clear() {
    varToStmtMap.clear();
    stmtToVarMap.clear();
}


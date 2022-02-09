#include "UseStore.h"

#include <cassert>

using namespace std;

UseStore::UseStore() {}

void UseStore::setUses(shared_ptr<StmtInfo> stmtInfo, VarRef varName) {
    StmtRef stmtNo = (stmtInfo.get())->reference;
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = varToStmtMap.find(varName);
    if (keyItr == varToStmtMap.end()) {
        varToStmtMap.insert(make_pair(varName, unordered_set<shared_ptr<StmtInfo>>{stmtInfo}));
    } else {
        keyItr->second.insert(stmtInfo);
    }
    auto keyItr2 = stmtToVarMap.find(stmtNo);
    if (keyItr2 == stmtToVarMap.end()) {
        stmtToVarMap.insert(make_pair(stmtNo, unordered_set<VarRef>{varName}));
    } else {
        keyItr2->second.insert(varName);
    }
}

bool UseStore::checkUses(shared_ptr<StmtInfo> stmtInfo, VarRef varName) {
    StmtRef stmtNo = (stmtInfo.get())->reference;
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = varToStmtMap.find(varName);
    if (keyItr == varToStmtMap.end()) return false;

    for (auto& itr : keyItr->second) {
        if (stmtInfo.get() == itr.get()) {
            return true;
        }
    }
    return false;
}

bool UseStore::checkUsesList(vector<shared_ptr<StmtInfo>> stmtInfoList, VarRef varName) {
    for (auto& itr : stmtInfoList) {
        if ((itr.get())->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");
        if (checkUses(itr, varName)) return true;
    }
    return false;
}

unordered_set<shared_ptr<StmtInfo>> UseStore::getUsesByVar(VarRef varName) {
    auto keyItr = varToStmtMap.find(varName);
    if (keyItr == varToStmtMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>{};
    } else {
        return keyItr->second;
    }
}

unordered_set<VarRef> UseStore::getUsesByStmt(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmtNo = (stmtInfo.get())->reference;
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = stmtToVarMap.find(stmtNo);
    if (keyItr == stmtToVarMap.end()) {
        return unordered_set<VarRef>{};
    } else {
        return keyItr->second;
    }
}

unordered_set<VarRef> UseStore::getUsesByStmtList(vector<shared_ptr<StmtInfo>> stmtInfoList) {
    unordered_set<VarRef> varSet;
    for (auto& itr : stmtInfoList) {
        if ((itr.get())->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");

        unordered_set<VarRef> stmtVarSet = getUsesByStmt(itr);
        varSet.insert(stmtVarSet.begin(), stmtVarSet.end());
    }
    return varSet;
}

unordered_set<ProcRef> UseStore::getProcUsesByStmtList(unordered_set<shared_ptr<StmtInfo>> stmtInfoList, ProcStore procStore) {
    unordered_set<ProcRef> procSet;
    for (auto &itr: stmtInfoList) {
        if (itr.get()->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");

        ProcRef procName = procStore.getProcByStmt(itr);
        if (!procName.empty()) procSet.insert(procName);
    }
    return procSet;
}

void UseStore::clear() {
    varToStmtMap.clear();
    stmtToVarMap.clear();
}

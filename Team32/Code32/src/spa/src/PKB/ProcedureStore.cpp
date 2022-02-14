#include "ProcedureStore.h"

ProcedureStore::ProcedureStore() {}

void ProcedureStore::setProc(ProcRef procName, vector<shared_ptr<StmtInfo>> idxList) {
    for (auto itr : idxList) {
        StmtRef stmtNo = itr->reference;
        if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

        auto keyItr1 = stmtToProcMap.find(stmtNo);
        if (keyItr1 != stmtToProcMap.end()) throw invalid_argument("Statement is already part of a procedure.");
        stmtToProcMap.insert(make_pair(stmtNo, procName));
    }

    auto keyItr = procMap.find(procName);
    if (keyItr == procMap.end()) {
        ProcRelation procRelation = { idxList, NULL };
        procMap.insert(make_pair(procName, procRelation));
    } else if (keyItr->second.idxList.empty()) {
        keyItr->second.idxList = idxList;
    } else {
        throw invalid_argument("Procedure already exists in Procedure Map.");
    }
}

void ProcedureStore::setCall(shared_ptr<StmtInfo> callStmt, ProcRef procName) {
    if (callStmt->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = procMap.find(procName);
    if (keyItr == procMap.end()) {
        ProcRelation procRelation = { vector<shared_ptr<StmtInfo>>() , callStmt };
        procMap.insert(make_pair(procName, procRelation));
    } else if (keyItr->second.callStmt == NULL) {
        keyItr->second.callStmt = callStmt;
    } else {
        throw invalid_argument("Procedure already has a call statement.");
    }
}

vector<shared_ptr<StmtInfo>> ProcedureStore::getStmtsByProc(ProcRef procName) {
    auto keyItr = procMap.find(procName);
    if (keyItr == procMap.end()) return vector<shared_ptr<StmtInfo>>{};
    return keyItr->second.idxList;
}

ProcRef ProcedureStore::getProcByCall(shared_ptr<StmtInfo> callStmt) {
    if (callStmt-> reference <= 0) throw invalid_argument("Statement number must be a positive integer.");
    if (callStmt->type != StmtType::Call) throw invalid_argument("Statement type must be a call statement.");

    for (auto& itr : procMap) {
        if (itr.second.callStmt == callStmt) return itr.first;
    }
    return "";
}

ProcRef ProcedureStore::getProcByStmt(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmtNo = stmtInfo->reference;
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = stmtToProcMap.find(stmtNo);
    if (keyItr == stmtToProcMap.end()) return "";
    return keyItr->second;
}

unordered_set<ProcRef> ProcedureStore::getProcListByStmtList(unordered_set<shared_ptr<StmtInfo>> stmtInfoList) {
    unordered_set<ProcRef> procSet;
    for (auto &itr: stmtInfoList) {
        if (itr->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");

        ProcRef procName = getProcByStmt(itr);
        if (!procName.empty()) procSet.insert(procName);
    }
    return procSet;
};

void ProcedureStore::clear() {
    procMap.clear();
}

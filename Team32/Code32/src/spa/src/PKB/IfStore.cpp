#include "IfStore.h"

IfStore::IfStore() {}

void IfStore::populateIfStore(shared_ptr<StmtInfo> ifInfo, unordered_set<shared_ptr<StmtInfo>> stmtsInfo) {
    auto if_itr = ifToStmtsMap.find(ifInfo->reference);
    if (if_itr != ifToStmtsMap.end()) {
        throw invalid_argument("If statement already has mapped statements");
    } else {
        ifToStmtsMap.insert(make_pair(ifInfo->reference, stmtsInfo));
    }

    for (auto& stmtInfo : stmtsInfo) {
        auto stmt_itr = stmtToIfsMap.find(stmtInfo->reference);
        if (stmt_itr != stmtToIfsMap.end()) {
            stmt_itr->second.insert(ifInfo);
        } else {
            stmtToIfsMap.insert(make_pair(stmtInfo->reference, unordered_set<shared_ptr<StmtInfo>>{ifInfo}));
        }
    }
}

unordered_set<shared_ptr<StmtInfo>> IfStore::getIfsFromStmts(unordered_set<shared_ptr<StmtInfo>> stmtsInfo) {
    unordered_set<shared_ptr<StmtInfo>> ifSet;
    for (auto& stmtInfo : stmtsInfo) {
        auto stmt_itr = stmtToIfsMap.find(stmtInfo->reference);
        if (stmt_itr != stmtToIfsMap.end()) {
            ifSet.insert(stmt_itr->second.begin(), stmt_itr->second.end());
        }
    }
    return ifSet;
}

unordered_set<shared_ptr<StmtInfo>> IfStore::getStmtsFromIf(shared_ptr<StmtInfo> ifInfo) {
    auto if_itr = ifToStmtsMap.find(ifInfo->reference);
    if (if_itr != ifToStmtsMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>();
    } else {
        return if_itr->second;
    }
}

void IfStore::clear() {
    stmtToIfsMap.clear();
    ifToStmtsMap.clear();
}
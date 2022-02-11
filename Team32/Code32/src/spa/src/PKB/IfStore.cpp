#include "IfStore.h"

IfStore::IfStore() {}

void IfStore::setIfsFromStmt(shared_ptr<StmtInfo> stmtInfo, unordered_set<shared_ptr<StmtInfo>> ifsInfo) {
    auto stmt_itr = stmtToIfsMap.find(stmtInfo->reference);
    if (stmt_itr != stmtToIfsMap.end()) {
        throw invalid_argument("Statement already has mapped If statements");
    }
    stmtToIfsMap.insert(make_pair(stmtInfo->reference, ifsInfo));
}

void IfStore::setStmtsFromIf(shared_ptr<StmtInfo> ifInfo, unordered_set<shared_ptr<StmtInfo>> stmtsInfo) {
    auto if_itr = ifToStmtsMap.find(ifInfo->reference);
    if (if_itr != ifToStmtsMap.end()) {
        throw invalid_argument("If statement already has mapped statements");
    }
    ifToStmtsMap.insert(make_pair(ifInfo->reference, stmtsInfo));
}

unordered_set<shared_ptr<StmtInfo>> IfStore::getIfsFromStmt(shared_ptr<StmtInfo> stmtInfo) {
    auto stmt_itr = stmtToIfsMap.find(stmtInfo->reference);
    if (stmt_itr != stmtToIfsMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>();
    } else {
        return stmt_itr->second;
    }
}

unordered_set<shared_ptr<StmtInfo>> IfStore::getStmtsFromIf(shared_ptr<StmtInfo> stmtInfo) {
    auto if_itr = ifToStmtsMap.find(stmtInfo->reference);
    if (if_itr != ifToStmtsMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>();
    } else {
        return if_itr->second;
    }
}
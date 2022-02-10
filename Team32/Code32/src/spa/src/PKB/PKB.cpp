#include "PKB.h"

using namespace std;

PKB::PKB() {}

StmtInfoPtrList PKB::getStatements() {
  StmtInfoPtrList stmtList;
  for (auto key_value : typeMap) {
    stmtList.push_back(key_value.second);
  }
  return stmtList;
}

void PKB::setParent(StmtRef stmtNo1, StmtRef stmtNo2) {
    shared_ptr<StmtInfo> stmtInfo1 = typeMap.at(stmtNo1);
    shared_ptr<StmtInfo> stmtInfo2 = typeMap.at(stmtNo2);
    parentStore.setParent(stmtInfo1, stmtInfo2);
}

shared_ptr<StmtInfo> PKB::getParent(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return parentStore.getParent(stmtInfo);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getChildren(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return parentStore.getChildren(stmtInfo);
}

bool PKB::checkParents(StmtRef stmtNo1, StmtRef stmtNo2) {
    shared_ptr<StmtInfo> stmtInfo1 = typeMap.at(stmtNo1);
    shared_ptr<StmtInfo> stmtInfo2 = typeMap.at(stmtNo2);
    return parentStore.isParentChild(stmtInfo1, stmtInfo2);
}

void PKB::setFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    shared_ptr<StmtInfo> stmtInfo1 = typeMap.at(stmtNo1);
    shared_ptr<StmtInfo> stmtInfo2 = typeMap.at(stmtNo2);
    followStore.setFollows(stmtInfo1, stmtInfo2);
}

bool PKB::checkFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    shared_ptr<StmtInfo> stmtInfo1 = typeMap.at(stmtNo1);
    shared_ptr<StmtInfo> stmtInfo2 = typeMap.at(stmtNo2);
    return followStore.checkFollows(stmtInfo1, stmtInfo2);
}

shared_ptr<StmtInfo> PKB::getFollower(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return followStore.getFollower(stmtInfo);
}

shared_ptr<StmtInfo> PKB::getFollowee(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return followStore.getFollowee(stmtInfo);
}

void PKB::setAssign(StmtRef stmtNo, VarRef variableLHS, string opTree) {
    return assignStore.setAssign(stmtNo, variableLHS, opTree);
}

void PKB::setProc(ProcRef procName, StmtRefList idxList) {
    StmtInfoPtrList stmtList;
    for (auto stmtRef : idxList) {
        stmtList.push_back(typeMap.at(stmtRef));
    }
    procStore.setProc(procName, stmtList);
}

void PKB::setCall(StmtRef stmt, ProcRef procName) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    procStore.setCall(stmtInfo, procName);
}

void PKB::setStmtType(StmtRef stmtNo, StmtType type) {
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = typeMap.find(stmtNo);
    if (keyItr == typeMap.end()) {
        StmtInfo stmtInfo = { stmtNo, type };
        auto stmtInfoPtr = make_shared<StmtInfo>(stmtInfo);
        typeMap.insert(make_pair(stmtNo, stmtInfoPtr));
    } else {
        throw invalid_argument("Statement type has already been assigned for this statement.");
    }
}

void PKB::setUses(StmtRef stmt, VarRef varName) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    useStore.setUses(stmtInfo, varName);
}

void PKB::setModifies(StmtRef stmt, VarRef varName) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    modifyStore.setModify(stmtInfo, varName);
}

bool PKB::checkUses(StmtRef stmt, VarRef varName) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return useStore.checkUses(stmtInfo, varName);
}

bool PKB::checkProcUses(ProcRef procName, VarRef varName) {
    vector<shared_ptr<StmtInfo>> procStmts = procStore.getStmtsByProc(procName);
    return useStore.checkUsesList(procStmts, varName);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getUsesByVar(VarRef varName) {
    return useStore.getUsesByVar(varName);
}

unordered_set<ProcRef> PKB::getProcUsesByVar(VarRef varName) {
    unordered_set<shared_ptr<StmtInfo>> stmtList = useStore.getUsesByVar(varName);
    return procStore.getProcListByStmtList(stmtList);
}

unordered_set<VarRef> PKB::getUsesByStmt(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return useStore.getUsesByStmt(stmtInfo);
}

unordered_set<VarRef> PKB::getUsesByProc(ProcRef procName) {
    vector<shared_ptr<StmtInfo>> procStmts = procStore.getStmtsByProc(procName);
    return useStore.getUsesByStmtList(procStmts);
}

bool PKB::checkModifies(StmtRef stmt, VarRef varName) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return modifyStore.checkModifies(stmtInfo, varName);
}

unordered_set<VarRef> PKB::getModifiesByProc(ProcRef procName) {
    vector<shared_ptr<StmtInfo>> procStmts = procStore.getStmtsByProc(procName);
    return modifyStore.getModifiesByStmtList(procStmts);
}

bool PKB::checkProcModifies(ProcRef procName, VarRef varName) {
    vector<shared_ptr<StmtInfo>> procStmts = procStore.getStmtsByProc(procName);
    return modifyStore.checkModifiesList(procStmts, varName);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getModifiesByVar(VarRef varName) {
    return modifyStore.getModifiesByVar(varName);
}

unordered_set<VarRef> PKB::getModifiesByStmt(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return modifyStore.getModifiesByStmt(stmtInfo);
}

unordered_set<ProcRef> PKB::getProcModifiesByVar(VarRef varName) {
    unordered_set<shared_ptr<StmtInfo>> stmtList = modifyStore.getModifiesByVar(varName);
    return procStore.getProcListByStmtList(stmtList);
}

ProcRef PKB::getProcFromCall(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = typeMap.at(stmt);
    return procStore.getProcByCall(stmtInfo);
}

/*
void PKB::populateComplexRelations() {
    parentStore.populateParentStar(numStatements);
}
 */

void PKB::clear() {
    parentStore.clear();
    followStore.clear();
    assignStore.clear();
    useStore.clear();
    modifyStore.clear();
    procStore.clear();
    typeMap.clear();
}

void PKB::setNumStatements(int size) {
    if (size < 0) throw invalid_argument("Size must be a positive integer.");
    this->numStatements = size;
}

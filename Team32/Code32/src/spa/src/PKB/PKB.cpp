#include "PKB.h"

using namespace std;

PKB::PKB() {}

StmtRefList PKB::getStatements() {
  StmtRefList stmtRefList;
  for (auto key_value : typeMap) {
    stmtRefList.push_back(key_value.first);
  }
  return stmtRefList;
}

void PKB::setParent(shared_ptr<StmtInfo> stmtNo1, shared_ptr<StmtInfo> stmtNo2) {
    parentStore.setParent(stmtNo1, stmtNo2);
}

shared_ptr<StmtInfo> PKB::getParent(shared_ptr<StmtInfo> stmt) {
    return parentStore.getParent(stmt);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getChildren(shared_ptr<StmtInfo> stmt) {
    return parentStore.getChildren(stmt);
}

bool PKB::checkParents(shared_ptr<StmtInfo> stmtNo1, shared_ptr<StmtInfo> stmtNo2) {
    return parentStore.isParentChild(stmtNo1, stmtNo2);
}

void PKB::setFollows(shared_ptr<StmtInfo> stmtNo1, shared_ptr<StmtInfo> stmtNo2) {
    followStore.setFollows(stmtNo1, stmtNo2);
}

bool PKB::checkFollows(shared_ptr<StmtInfo> stmtNo1, shared_ptr<StmtInfo> stmtNo2) {
    return followStore.checkFollows(stmtNo1, stmtNo2);
}

shared_ptr<StmtInfo> PKB::getFollower(shared_ptr<StmtInfo> stmt) {
    return followStore.getFollower(stmt);
}

shared_ptr<StmtInfo> PKB::getFollowee(shared_ptr<StmtInfo> stmt) {
    return followStore.getFollowee(stmt);
}

void PKB::setAssign(StmtRef stmtNo, VarRef variableLHS, string opTree) {
    return assignStore.setAssign(stmtNo, variableLHS, opTree);
}

void PKB::setProc(ProcRef proc_name, vector<StmtRef> idxList) {
    for (auto itr : idxList) {
        checkInvalidStmt(itr);
    }
    auto keyItr = procMap.find(proc_name);
    if (keyItr == procMap.end()) {
        procMap.insert(make_pair(proc_name, idxList));
    } else {
        keyItr->second = idxList;
    }
}

void PKB::setStmtType(StmtRef stmtNo, StmtType type) {
    checkInvalidStmt(stmtNo);

    auto keyItr = typeMap.find(stmtNo);
    if (keyItr == typeMap.end()) {
        typeMap.insert(make_pair(stmtNo, type));
    } else {
        keyItr->second = type;
    }
}

void PKB::setUses(shared_ptr<StmtInfo> stmt, VarRef var_name) {
    useStore.setUses(stmt, var_name);
}

void PKB::setModifies(shared_ptr<StmtInfo> stmt, VarRef var_name) {
    modifyStore.setModify(stmt, var_name);
}

void PKB::populateComplexRelations() {
    parentStore.populateParentStar(numStatements);
}

void PKB::clear() {
    parentStore.clear();
    followStore.clear();
    assignStore.clear();
    useStore.clear();
    modifyStore.clear();
    procMap.clear();
    typeMap.clear();
}

void PKB::setNumStatements(int size) {
    if (size < 0) throw invalid_argument("Size must be a positive integer.");
    this->numStatements = size;
}

void PKB::checkInvalidStmt(StmtRef stmtNo1) {
    if (stmtNo1 <= 0) throw invalid_argument("Statement number must be a positive integer.");
}

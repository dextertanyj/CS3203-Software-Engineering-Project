#include "PKB.h"

using namespace std;

PKB::PKB() {}

void PKB::setParent(StmtRef stmtNo1, StmtRef stmtNo2) {
    checkInvalidStmts(stmtNo1, stmtNo2);
    parentStore.setParent(stmtNo1, stmtNo2);
}

StmtRef PKB::getParent(StmtRef stmtNo) {
    checkInvalidStmt(stmtNo);
    return parentStore.getParent(stmtNo);
}

unordered_set<StmtRef> PKB::getChildren(StmtRef stmtNo) {
    checkInvalidStmt(stmtNo);
    return parentStore.getChildren(stmtNo);
}

bool PKB::checkParents(StmtRef stmtNo1, StmtRef stmtNo2) {
    try {
        checkInvalidStmts(stmtNo1, stmtNo2);
    } catch (invalid_argument) {
        return false;
    }
    return parentStore.isParentChild(stmtNo1, stmtNo2);
}

void PKB::setFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    checkInvalidStmts(stmtNo1, stmtNo2);
    followStore.setFollows(stmtNo1, stmtNo2);
}

bool PKB::checkFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    try {
        checkInvalidStmts(stmtNo1, stmtNo2);
    } catch (invalid_argument) {
        return false;
    }
    return followStore.checkFollows(stmtNo1, stmtNo2);
}

StmtRef PKB::getFollower(StmtRef stmtNo) {
    checkInvalidStmt(stmtNo);
    return followStore.getFollower(stmtNo);
}

StmtRef PKB::getFollowee(StmtRef stmtNo) {
    checkInvalidStmt(stmtNo);
    return followStore.getFollowee(stmtNo);
}

void PKB::setAssign(StmtRef stmtNo, VarRef variableLHS, string opTree) {
    checkInvalidStmt(stmtNo);
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

void PKB::setUses(StmtRef stmtNo, VarRef var_name) {
    checkInvalidStmt(stmtNo);
    useStore.setUses(stmtNo, var_name);
}

void PKB::setModifies(StmtRef stmtNo, VarRef var_name) {
    checkInvalidStmt(stmtNo);
    modifyStore.setModify(stmtNo, var_name);
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

void PKB::checkInvalidStmts(StmtRef stmtNo1, StmtRef stmtNo2) {
    if (stmtNo1 >= stmtNo2) throw invalid_argument("Second statement must come after the first statement.");
    if (stmtNo1 < 0 || stmtNo2 < 0) throw invalid_argument("Statement number must be a positive integer.");
}

void PKB::checkInvalidStmt(StmtRef stmtNo1) {
    if (stmtNo1 < 0) throw invalid_argument("Statement number must be a positive integer.");
}

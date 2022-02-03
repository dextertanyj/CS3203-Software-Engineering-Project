#include "PKB.h"

using namespace std;

PKB::PKB() {}

bool PKB::setParent(StmtRef stmtNo1, StmtRef stmtNo2) {
    if (checkInvalidStmts(stmtNo1, stmtNo2)) {
        return false;
    }
    parentStore.setParent(stmtNo1, stmtNo2);
    return true;
}

StmtRef PKB::getParent(StmtRef stmtNo) {
    if (stmtNo < 0) {
        return -1;
    }
    return parentStore.getParent(stmtNo);
}

unordered_set<StmtRef> PKB::getChildren(StmtRef stmtNo) {
    if (stmtNo < 0) {
        return unordered_set<StmtRef>{};
    }
    return parentStore.getChildren(stmtNo);
}

bool PKB::checkParents(StmtRef stmtNo1, StmtRef stmtNo2) {
    if (checkInvalidStmts(stmtNo1, stmtNo2)) {
        return false;
    }
    return parentStore.isParentChild(stmtNo1, stmtNo2);
}

bool PKB::setFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    if (checkInvalidStmts(stmtNo1, stmtNo2)) {
        return false;
    }
    followStore.setFollows(stmtNo1, stmtNo2);
    return true;
}

bool PKB::checkFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    if (checkInvalidStmts(stmtNo1, stmtNo2)) {
        return false;
    }
    return followStore.checkFollows(stmtNo1, stmtNo2);
}

StmtRef PKB::getFollower(StmtRef stmtNo) {
    if (stmtNo < 0) {
        return -1;
    }
    return followStore.getFollower(stmtNo);
}

StmtRef PKB::getFollowee(StmtRef stmtNo) {
    if (stmtNo < 0) {
        return -1;
    }
    return followStore.getFollowee(stmtNo);
}

bool PKB::checkInvalidStmts(StmtRef stmtNo1, StmtRef stmtNo2) {
    return (stmtNo1 >= stmtNo2 || stmtNo1 < 0 || stmtNo2 < 0);
}

void PKB::setProc(ProcRef proc_name, vector<StmtRef> idxList) {
    auto keyItr = procMap.find(proc_name);
    if (keyItr == procMap.end()) {
        procMap.insert(make_pair(proc_name, idxList));
    } else {
        keyItr->second = idxList;
    }
}

void PKB::setStmtType(StmtRef stmtNo, StmtType type) {
    auto keyItr = typeMap.find(stmtNo);
    if (keyItr == typeMap.end()) {
        typeMap.insert(make_pair(stmtNo, type));
    } else {
        keyItr->second = type;
    }
}

void PKB::setUses(StmtRef stmtNo, VarRef var_name) {
    useStore.setUses(stmtNo, var_name);
}

void PKB::setModifies(StmtRef stmtNo, VarRef var_name) {
    modifyStore.setModify(stmtNo, var_name);
}

void PKB::clear() {
    parentStore.clear();
    followStore.clear();
    procMap.clear();
    typeMap.clear();
}

void PKB::setNumStatements(int size) {
    parentStore.setNumStatements(size);
}

void PKB::populateComplexRelations() {
    parentStore.populateParentStar();
}

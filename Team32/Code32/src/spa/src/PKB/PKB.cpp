
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "ParentStore.h"
#include "../TNode.h"

/*
int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST (PROC p){
	return nullptr;
}
 */

ParentStore PKB::parentStore;
FollowStore PKB::followStore;

PKB::PKB() {}

bool PKB::setParent(int stmtNo1, int stmtNo2) {
    if (checkInvalidStmts(stmtNo1, stmtNo2)) {
        return false;
    }
    parentStore.setParent(stmtNo1, stmtNo2);
    return true;
}

int PKB::getParent(int stmtNo) {
    if (stmtNo < 0) {
        return -1;
    }
    return parentStore.getParent(stmtNo);
}

unordered_set<int> PKB::getChildren(int stmtNo) {
    if (stmtNo < 0) {
        return unordered_set<int>{};
    }
    return parentStore.getChildren(stmtNo);
}

bool PKB::checkParents(int stmtNo1, int stmtNo2) {
    if (checkInvalidStmts(stmtNo1, stmtNo2)) {
        return false;
    }
    return parentStore.isParentChild(stmtNo1, stmtNo2);
}

bool PKB::setFollows(int stmtNo1, int stmtNo2) {
    if (checkInvalidStmts(stmtNo1, stmtNo2)) {
        return false;
    }
    followStore.setFollows(stmtNo1, stmtNo2);
    return true;
}

bool PKB::checkFollows(int stmtNo1, int stmtNo2) {
    if (checkInvalidStmts(stmtNo1, stmtNo2)) {
        return false;
    }
    return followStore.checkFollows(stmtNo1, stmtNo2);
}

int PKB::getFollower(int stmtNo) {
    if (stmtNo < 0) {
        return -1;
    }
    return followStore.getFollower(stmtNo);
}

int PKB::getFollowee(int stmtNo) {
    if (stmtNo < 0) {
        return -1;
    }
    return followStore.getFollowee(stmtNo);
}

bool PKB::checkInvalidStmts(int stmtNo1, int stmtNo2) {
    if (stmtNo1 > stmtNo2 || stmtNo1 < 0 || stmtNo2 < 0) {
        return true;
    } else {
        return false;
    }
}

void PKB::setProc(std::string proc_name, vector<int> idxList) {
    auto keyItr = procMap.find(proc_name);
    if (keyItr == procMap.end()) {
        procMap.insert(std::make_pair(proc_name, idxList));
    } else {
        keyItr->second = idxList;
    }
}

void PKB::setStmtType(int stmtNo, stmtType type) {
    auto keyItr = typeMap.find(stmtNo);
    if (keyItr == typeMap.end()) {
        typeMap.insert(std::make_pair(stmtNo, type));
    } else {
        keyItr->second = type;
    }
}

void PKB::setUses(int stmtNo, std::string var_name) {
    useStore.setUses(stmtNo, var_name);
}

void PKB::setModifies(int stmtNo, std::string var_name) {
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


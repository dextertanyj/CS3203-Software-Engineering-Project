#include "ParentStore.h"

using namespace std;

ParentStore::ParentStore() {}

void ParentStore::setParent(int parentStmt, int childStmt) {
    auto keyItr = parentMap.find(parentStmt);
    if (keyItr == parentMap.end()) {
        // parent does not exist as key
        ParentRelation parentRelation = { {childStmt}, {}, {}, -1 };
        parentMap.insert(make_pair(parentStmt, parentRelation));
    } else {
        // parent exists as key, add childStmtNo to vector of children
        ParentRelation parentRelation = keyItr->second;
        parentRelation.childSet.insert(childStmt);
    }
    auto keyItr2 = parentMap.find(childStmt);
    if (keyItr2 == parentMap.end()) {
        ParentRelation parentRelation = { {}, {}, {}, parentStmt };
        parentMap.insert(make_pair(childStmt, parentRelation));
    } else {
        keyItr2->second.parent = parentStmt;
    }
}

bool ParentStore::isParentChild(int parentStmt, int childStmt) {
    auto keyItr = parentMap.find(childStmt);
    if (keyItr == parentMap.end()) {
        return false;
    }
    int parent = keyItr->second.parent;
    if (parent == parentStmt) {
        return true;
    } else {
        return false;
    }
}

int ParentStore::getParent(int stmt) {
    auto keyItr = parentMap.find(stmt);
    if (keyItr == parentMap.end()) {
        return -1;
    }
    return keyItr->second.parent;
}

unordered_set<int> ParentStore::getChildren(int stmt) {
    for(auto& itr : parentMap) {
        if (itr.first == stmt) {
            return itr.second.childSet;
        }
    }
    return unordered_set<int>{};
}

void ParentStore::populateParentStar() {
    if (numStatements >= 2) {
        for (int i = 2; i < numStatements; i++) {
            populateParentStarSet(i);
        }
    }
}

void ParentStore::populateParentStarSet(int stmtNo) {
    auto keyItr = parentMap.find(stmtNo);
    recursivelyAddParent(stmtNo, keyItr->second.parentStarSet);
}

void ParentStore::recursivelyAddParent(int stmtNo, unordered_set<int> &parentStarSet) {
    auto keyItr = parentMap.find(stmtNo);
    int parent = keyItr->second.parent;
    if (parent == -1) {
        return;
    }
    unordered_set<int> parentOfParentSet = parentMap.find(parent)->second.parentStarSet;
    if (parentOfParentSet.empty()) {
        parentStarSet.insert(parent);
        recursivelyAddParent(parent, parentStarSet);
    } else {
        parentStarSet.insert(parent);
        parentStarSet.insert(parentOfParentSet.begin(), parentOfParentSet.end());
        return;
    }
}

void ParentStore::setNumStatements(int size) {
    numStatements = size;
}

void ParentStore::clear() {
    parentMap.clear();
}




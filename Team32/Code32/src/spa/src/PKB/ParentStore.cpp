#include "ParentStore.h"

using namespace std;

ParentStore::ParentStore() {}

void ParentStore::setParent(StmtRef parentStmt, StmtRef childStmt) {
    assert(parentStmt > 0 && childStmt > 0);
    assert(parentStmt < childStmt);
        
    auto keyItr = parentMap.find(parentStmt);
    if (keyItr == parentMap.end()) {
        // If parent does not exist as key, create and store into parentMap.
        ParentRelation parentRelation = { {childStmt}, {}, {}, -1 };
        parentMap.insert(make_pair(parentStmt, parentRelation));
    } else {
        // If parent already exists as key, add childStmtNo to vector of children
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

bool ParentStore::isParentChild(StmtRef parentStmt, StmtRef childStmt) {
    assert(parentStmt > 0 && childStmt > 0);
    assert(parentStmt < childStmt);

    auto keyItr = parentMap.find(childStmt);
    if (keyItr == parentMap.end()) {
        return false;
    }
    StmtRef parentStmtInStore = keyItr->second.parent;
    return parentStmtInStore == parentStmt;
}

StmtRef ParentStore::getParent(StmtRef stmt) {
    assert(stmt > 0);

    auto keyItr = parentMap.find(stmt);
    if (keyItr == parentMap.end()) {
        return -1;
    }
    return keyItr->second.parent;
}

unordered_set<StmtRef> ParentStore::getChildren(StmtRef stmt) {
    assert(stmt > 0);

    for(auto& itr : parentMap) {
        if (itr.first == stmt) {
            return itr.second.childSet;
        }
    }
    return unordered_set<StmtRef>{};
}

void ParentStore::populateParentStar(int numStatements) {
    if (numStatements >= 2) {
        for (StmtRef i = 2; i < numStatements; i++) {
            populateParentStarSet(i);
        }
    }
}

void ParentStore::populateParentStarSet(StmtRef stmtNo) {
    auto keyItr = parentMap.find(stmtNo);
    recursivelyAddParent(stmtNo, keyItr->second.parentStarSet);
}

void ParentStore::recursivelyAddParent(StmtRef stmtNo, unordered_set<StmtRef> &parentStarSet) {
    auto keyItr = parentMap.find(stmtNo);
    StmtRef parent = keyItr->second.parent;
    if (parent == -1) {
        return;
    }
    unordered_set<StmtRef> parentOfParentSet = parentMap.find(parent)->second.parentStarSet;
    if (parentOfParentSet.empty()) {
        parentStarSet.insert(parent);
        recursivelyAddParent(parent, parentStarSet);
    } else {
        parentStarSet.insert(parent);
        parentStarSet.insert(parentOfParentSet.begin(), parentOfParentSet.end());
        return;
    }
}

void ParentStore::clear() {
    parentMap.clear();
}




#include "ParentStore.h"

#include <cassert>

using namespace std;

ParentStore::ParentStore() {}

void ParentStore::setParent(shared_ptr<StmtInfo> parentStmtInfo, shared_ptr<StmtInfo> childStmtInfo) {
    StmtRef parentStmt = parentStmtInfo->reference;
    StmtRef childStmt = childStmtInfo->reference;

    if (parentStmt >= childStmt) throw invalid_argument("Second statement must come after the first statement.");
    if (parentStmt <= 0 || childStmt <= 0) throw invalid_argument("Statement number must be a positive integer.");
        
    auto keyItr = parentMap.find(parentStmt);
    if (keyItr == parentMap.end()) {
        // If parent does not exist as key, create and store into parentMap.
        ParentRelation parentRelation = {{childStmtInfo}, {}, {}, NULL};
        parentMap.insert(make_pair(parentStmt, parentRelation));
    } else {
        // If parent already exists as key, add childStmtNo to set of children
        keyItr->second.childSet.insert(childStmtInfo);
    }
    auto keyItr2 = parentMap.find(childStmt);
    if (keyItr2 == parentMap.end()) {
        ParentRelation parentRelation = { {}, {}, {}, parentStmtInfo };
        parentMap.insert(make_pair(childStmt, parentRelation));
    } else {
        keyItr2->second.parent = parentStmtInfo;
    }
}

bool ParentStore::isParentChild(shared_ptr<StmtInfo> parentStmtInfo, shared_ptr<StmtInfo> childStmtInfo) {
    StmtRef parentStmt = parentStmtInfo->reference;
    StmtRef childStmt = childStmtInfo->reference;

    if (parentStmt <= 0 || childStmt <= 0) throw invalid_argument("Statement number must be a positive integer.");
    if (parentStmt >= childStmt) return false;

    auto keyItr = parentMap.find(childStmt);
    if (keyItr == parentMap.end()) {
        return false;
    }
    shared_ptr<StmtInfo> parentStmtInStore = keyItr->second.parent;
    return parentStmtInStore == parentStmtInfo;
}

shared_ptr<StmtInfo> ParentStore::getParent(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmt = stmtInfo->reference;

    if (stmt <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = parentMap.find(stmt);
    if (keyItr == parentMap.end()) {
        return make_shared<StmtInfo>();
    }
    return keyItr->second.parent;
}

unordered_set<shared_ptr<StmtInfo>> ParentStore::getChildren(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmt = stmtInfo->reference;

    if (stmt <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = parentMap.find(stmt);
    if (keyItr == parentMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>{};
    }
    return keyItr->second.childSet;
}

/*
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
 */

void ParentStore::clear() {
    parentMap.clear();
}




//
// Created by Yanzieee on 26/1/2022.
//

#include "ParentStore.h"
#include <unordered_map>
#include <unordered_set>
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

ParentStore::ParentStore() {}

void ParentStore::setParent(int parentStmt, int childStmt) {
    auto keyItr = parentMap.find(parentStmt);
    if (keyItr == parentMap.end()) {
        // parent does not exist as key
        ParentRelation parentRelation = { {childStmt}, {}, {} };
        parentMap.insert(std::make_pair(parentStmt, parentRelation));
    } else {
        // parent exists as key, add childStmtNo to vector of children
        ParentRelation parentRelation = keyItr->second;
        parentRelation.childSet.insert(childStmt);
    }
}

bool ParentStore::isParentChild(int parentStmt, int childStmt) {
    auto keyItr = parentMap.find(parentStmt);
    if (keyItr == parentMap.end()) {
        return false;
    }
    unordered_set<int> childSet = keyItr->second.childSet;
    if (childSet.find(childStmt) != childSet.end()) {
        // child exists in parent's childList
        return true;
    } else {
        return false;
    }
}

int ParentStore::getParent(int stmt) {
    for(auto& itr : parentMap) {
        unordered_set<int> childSet = itr.second.childSet;
        if (childSet.find(stmt) != childSet.end()) {
            // child exists in parent's childList
            return itr.first;
        }
    }
    return -1;
}

unordered_set<int> ParentStore::getChildren(int stmt) {
    for(auto& itr : parentMap) {
        if (itr.first == stmt) {
            return itr.second.childSet;
        }
    }
    return unordered_set<int>{};
}

void ParentStore::clear() {
    parentMap.clear();
}




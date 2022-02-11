#include "ParentStore.h"

using namespace std;

ParentStore::ParentStore() {}

void ParentStore::setParent(shared_ptr<StmtInfo> parentStmtInfo, shared_ptr<StmtInfo> childStmtInfo) {
	StmtRef parentStmt = parentStmtInfo->reference;
	StmtRef childStmt = childStmtInfo->reference;

	if (parentStmt >= childStmt) throw invalid_argument("Second statement must come after the first statement.");
	if (parentStmt <= 0 || childStmt <= 0) throw invalid_argument("Statement number must be a positive integer.");

	auto key_itr_parent_stmt = parentMap.find(parentStmt);
	auto key_itr_child_stmt = parentMap.find(childStmt);

	// Check if child already has a parent.
	if (key_itr_child_stmt != parentMap.end()) {
		shared_ptr<StmtInfo> parent_of_child_ptr = key_itr_child_stmt->second.parent;
		if (parent_of_child_ptr != nullptr) {
			throw invalid_argument("Child statement already has parent assigned.");
		}
	}
	// If above are valid, then proceed to add child stmt to parent and parent to child.
	if (key_itr_parent_stmt == parentMap.end()) {
		// If parent does not exist as key, create and store into parentMap.
		ParentRelation parent_relation = {{childStmtInfo}, {}, {}, nullptr};
		parentMap.insert(make_pair(parentStmt, parent_relation));
	} else {
		// If parent already exists as key, add childStmtNo to set of children.
		key_itr_parent_stmt->second.childSet.insert(childStmtInfo);
	}
	// After adding child to parent, below we add parent to child.
	if (key_itr_child_stmt == parentMap.end()) {  // Child might not yet be added to parent map.
		ParentRelation parent_relation = {{}, {}, {}, parentStmtInfo};
		parentMap.insert(make_pair(childStmt, parent_relation));
	} else {
		key_itr_child_stmt->second.parent = parentStmtInfo;
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
	if (stmt <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto key_parent = parentMap.find(stmt);
	// If statement is not even in parentMap, then return point with StmtRef -1 and StmtType unknown.
	if (key_parent != parentMap.end() && key_parent->second.parent != nullptr) {
		return key_parent->second.parent;
	}
	return nullptr;
}

unordered_set<shared_ptr<StmtInfo>> ParentStore::getChildren(shared_ptr<StmtInfo> stmtInfo) {
	StmtRef stmt = stmtInfo->reference;

	if (stmt <= 0) throw invalid_argument("Statement number must be a positive integer.");

	auto keyItr = parentMap.find(stmt);
	if (keyItr == parentMap.end()) {
		return unordered_set<shared_ptr<StmtInfo>>();
	}
	return keyItr->second.childSet;
}

unordered_set<shared_ptr<StmtInfo>> ParentStore::getParentStar(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmt = stmtInfo->reference;

    if (stmt <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = parentMap.find(stmt);
    if (keyItr == parentMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>();
    }
    return keyItr->second.parentStarSet;
}

unordered_set<shared_ptr<StmtInfo>> ParentStore::getChildStar(shared_ptr<StmtInfo> stmtInfo) {
    StmtRef stmt = stmtInfo->reference;

    if (stmt <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = parentMap.find(stmt);
    if (keyItr == parentMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>();
    }
    return keyItr->second.childStarSet;
}


void ParentStore::populateParent(int numStatements) {
    if (numStatements < 1) throw invalid_argument("Cannot populate without any statements");

    for (StmtRef i = 1; i <= numStatements; i++) {
        populateParentStarSet(i);
    }
    for (StmtRef i = numStatements; i >= 1; i--) {
        populateChildStarSet(i);
    }
}

void ParentStore::populateParentStarSet(StmtRef stmtNo) {
    auto keyItr = parentMap.find(stmtNo);
    if (keyItr != parentMap.end()) {
        recursivelyAddParent(stmtNo, keyItr->second.parentStarSet);
    }
}

void ParentStore::populateChildStarSet(StmtRef stmtNo) {
    auto keyItr = parentMap.find(stmtNo);
    if (keyItr != parentMap.end()) {
        recursivelyAddChild(stmtNo, keyItr->second.childStarSet);
    }
}


void ParentStore::recursivelyAddParent(StmtRef stmtNo, unordered_set<shared_ptr<StmtInfo>>& parentStarSet) {
    auto keyItr = parentMap.find(stmtNo);
    shared_ptr<StmtInfo> parent = keyItr->second.parent;
    if (parent == nullptr) {
        return;
    }
    unordered_set<shared_ptr<StmtInfo>> parentStarOfParentSet = parentMap.find(parent->reference)->second.parentStarSet;
//    if (parentOfParentSet.empty()) {
//        parentStarSet.insert(parent);
//        recursivelyAddParent(parent->reference, parentStarSet);
//    }
        parentStarSet.insert(parent);
        parentStarSet.insert(parentStarOfParentSet.begin(), parentStarOfParentSet.end());
    }

void ParentStore::recursivelyAddChild(StmtRef stmtNo, unordered_set<shared_ptr<StmtInfo>>& childStarSet) {
    auto keyItr = parentMap.find(stmtNo);
    unordered_set<shared_ptr<StmtInfo>> childSet = keyItr->second.childSet;
    if (childSet.empty()) {
        return;
    }
    for (auto& child : childSet) {
        unordered_set<shared_ptr<StmtInfo>> childStarSetOfChild = parentMap.find(child->reference)->second.childStarSet;
        childStarSet.insert(child);
        childStarSet.insert(childStarSetOfChild.begin(), childStarSetOfChild.end());
    }
}

void ParentStore::clear() { parentMap.clear(); }

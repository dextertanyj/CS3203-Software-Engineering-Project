#include "ParentStore.h"

using namespace std;

ParentStore::ParentStore() {}

void ParentStore::setParent(shared_ptr<StmtInfo> parent, shared_ptr<StmtInfo> child) {
	StmtRef parent_idx = parent->reference;
	StmtRef child_idx = child->reference;

	if (parent_idx >= child_idx) {
		throw invalid_argument("Second statement must come after the first statement");
	}

	auto parent_iter = parentMap.find(parent_idx);
	if (parent_iter == parentMap.end()) {
		// Parent does not exist in map
		ParentRelation parent_relation = {parent, nullptr, {child}, {}, {}};
		parentMap.insert(make_pair(parent_idx, parent_relation));
	} else {
		parent_iter->second.childSet.insert(child);
	}

	auto child_iter = parentMap.find(child_idx);
	if (child_iter == parentMap.end()) {
		ParentRelation child_relation = {child, parent, {}, {}, {}};
		parentMap.insert(make_pair(child_idx, child_relation));
	} else if (child_iter->second.parent == nullptr) {
		child_iter->second.parent = parent;
	} else {
		throw invalid_argument("Child statement already has parent assigned.");
	}
}

bool ParentStore::isParentChild(StmtRef parent, StmtRef child) {
	if (parent <= 0 || child <= 0) throw invalid_argument("Statement number must be a positive integer.");
	if (parent >= child) return false;

	auto iter = parentMap.find(child);
	if (iter == parentMap.end() || iter->second.parent == nullptr) {
		return false;
	}
	return iter->second.parent->reference == parent;
}

shared_ptr<StmtInfo> ParentStore::getParent(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto iter = parentMap.find(index);
	// If statement is not even in parentMap, then return point with StmtRef -1 and StmtType unknown.
	if (iter != parentMap.end() && iter->second.parent != nullptr) {
		return iter->second.parent;
	}
	return nullptr;
}

unordered_set<shared_ptr<StmtInfo>> ParentStore::getChildren(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto iter = parentMap.find(index);
	if (iter == parentMap.end()) {
		return {};
	}
	return iter->second.childSet;
}

unordered_set<shared_ptr<StmtInfo>> ParentStore::getParentStar(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}

	auto iter = parentMap.find(index);
	if (iter == parentMap.end()) {
		return {};
	}
	return iter->second.parentStarSet;
}

unordered_set<shared_ptr<StmtInfo>> ParentStore::getChildStar(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}

	auto iter = parentMap.find(index);
	if (iter == parentMap.end()) {
		return {};
	}
	return iter->second.childStarSet;
}

void ParentStore::optimize() {
	for (auto& item : parentMap) {
		if (item.second.parent == nullptr) {
			ParentStore::populateTransitive(item.second, {});
		}
	}
}

unordered_set<shared_ptr<StmtInfo>> ParentStore::populateTransitive(ParentRelation& current, unordered_set<shared_ptr<StmtInfo>> previous) {
	current.parentStarSet.insert(previous.begin(), previous.end());
	previous.insert(current.node);
	unordered_set<shared_ptr<StmtInfo>> result;
	for (const shared_ptr<StmtInfo>& child : current.childSet) {
		auto relation = parentMap.find(child->reference);
		unordered_set<shared_ptr<StmtInfo>> transitive_children = populateTransitive(relation->second, previous);
		result.insert(transitive_children.begin(), transitive_children.end());
	}
	current.childStarSet.insert(result.begin(), result.end());
	result.insert(current.node);
	return result;
}

void ParentStore::clear() { parentMap.clear(); }

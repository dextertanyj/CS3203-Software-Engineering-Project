#include "PKB/ParentPKB.h"

ParentPKB::ParentPKB(shared_ptr<StmtInfo> self) : self(self) {}

void ParentPKB::insertForward(shared_ptr<StmtInfo> parent) {
	if (this->parent != nullptr) {
		throw "This statement already has a parent";
	}
	this->parent = parent;
}

void ParentPKB::insertReverse(shared_ptr<StmtInfo> child) { this->children.insert(child); }

void ParentPKB::appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> parents) {
	this->parent_transitive.insert(parents.begin(), parents.end());
}

void ParentPKB::appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> children) {
	this->children_transitive.insert(children.begin(), children.end());
}

unordered_set<shared_ptr<StmtInfo>> ParentPKB::getForward() {
	if (parent == nullptr) {
		return {};
	}
	return {parent};
}

unordered_set<shared_ptr<StmtInfo>> ParentPKB::getReverse() { return children; }

unordered_set<shared_ptr<StmtInfo>> ParentPKB::getForwardTransitive() { return parent_transitive; }

unordered_set<shared_ptr<StmtInfo>> ParentPKB::getReverseTransitive() { return children_transitive; }

void ParentPKB::optimize(StatementRelationStore<ParentPKB>& store) {
	for (auto& item : store.map) {
		if (item.second.parent == nullptr) {
			ParentPKB::populateTransitive(store, item.second, {});
		}
	}
}

unordered_set<shared_ptr<StmtInfo>> ParentPKB::populateTransitive(StatementRelationStore<ParentPKB>& store, ParentPKB& current,
                                                                  unordered_set<shared_ptr<StmtInfo>> previous) {
	current.appendForwardTransitive(previous);
	previous.insert(current.self);
	unordered_set<shared_ptr<StmtInfo>> result;
	for (const shared_ptr<StmtInfo>& child : current.children) {
		auto relation = store.map.find(child->reference);
		unordered_set<shared_ptr<StmtInfo>> transitive_children = populateTransitive(store, relation->second, previous);
		result.insert(transitive_children.begin(), transitive_children.end());
	}
	current.appendReverseTransitive(result);
	result.insert(current.self);
	return result;
}
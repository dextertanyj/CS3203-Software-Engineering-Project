#include "PKB/ParentRelation.h"

#include <stdexcept>
#include <utility>

ParentRelation::ParentRelation(shared_ptr<StmtInfo> self) : self(std::move(self)) {}

void ParentRelation::insertForward(const shared_ptr<StmtInfo>& parent) {
	if (self->reference <= parent->reference) {
		throw invalid_argument("Statement out of order");
	}
	if (this->parent != nullptr) {
		throw invalid_argument("This statement already has a parent");
	}
	this->parent = parent;
}

void ParentRelation::insertReverse(const shared_ptr<StmtInfo>& child) {
	if (self->reference >= child->reference) {
		throw invalid_argument("Statement out of order");
	}
	this->children.insert(child);
}

void ParentRelation::appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> parents) {
	for (const auto& parent : parents) {
		if (self->reference <= parent->reference) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->parent_transitive.insert(parents.begin(), parents.end());
}

void ParentRelation::appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> children) {
	for (const auto& child : children) {
		if (self->reference >= child->reference) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->children_transitive.insert(children.begin(), children.end());
}

unordered_set<shared_ptr<StmtInfo>> ParentRelation::getForward() {
	if (parent == nullptr) {
		return {};
	}
	return {parent};
}

unordered_set<shared_ptr<StmtInfo>> ParentRelation::getReverse() { return children; }

unordered_set<shared_ptr<StmtInfo>> ParentRelation::getForwardTransitive() { return parent_transitive; }

unordered_set<shared_ptr<StmtInfo>> ParentRelation::getReverseTransitive() { return children_transitive; }

void ParentRelation::optimize(StatementRelationStore<ParentRelation>& store) {
	for (auto& item : store.map) {
		if (item.second.parent == nullptr) {
			ParentRelation::populateTransitive(store, item.second, {});
		}
	}
}

unordered_set<shared_ptr<StmtInfo>> ParentRelation::populateTransitive(StatementRelationStore<ParentRelation>& store,
                                                                       ParentRelation& current,
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
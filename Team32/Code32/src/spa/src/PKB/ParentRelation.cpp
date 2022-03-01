#include "PKB/ParentRelation.h"

#include <stdexcept>
#include <utility>

PKB::ParentRelation::ParentRelation(shared_ptr<StmtInfo> self) : self(std::move(self)) {}

void PKB::ParentRelation::insertForward(const shared_ptr<StmtInfo>& parent_to_insert) {
	if (self->getIdentifier() <= parent_to_insert->getIdentifier()) {
		throw invalid_argument("Statement out of order");
	}
	if (this->parent != nullptr) {
		throw invalid_argument("This statement already has a parent_to_insert");
	}
	this->parent = parent_to_insert;
}

void PKB::ParentRelation::insertReverse(const shared_ptr<StmtInfo>& child_to_insert) {
	if (self->getIdentifier() >= child_to_insert->getIdentifier()) {
		throw invalid_argument("Statement out of order");
	}
	this->children.insert(child_to_insert);
}

void PKB::ParentRelation::appendForwardTransitive(StmtInfoPtrSet parents_to_insert) {
	for (const auto& parent_to_insert : parents_to_insert) {
		if (self->getIdentifier() <= parent_to_insert->getIdentifier()) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->parent_transitive.insert(parents_to_insert.begin(), parents_to_insert.end());
}

void PKB::ParentRelation::appendReverseTransitive(StmtInfoPtrSet children_to_insert) {
	for (const auto& child : children_to_insert) {
		if (self->getIdentifier() >= child->getIdentifier()) {
			throw invalid_argument("Statement out of order");
		}
	}
	this->children_transitive.insert(children_to_insert.begin(), children_to_insert.end());
}

StmtInfoPtrSet PKB::ParentRelation::getForward() {
	if (parent == nullptr) {
		return {};
	}
	return {parent};
}

StmtInfoPtrSet PKB::ParentRelation::getReverse() { return children; }

StmtInfoPtrSet PKB::ParentRelation::getForwardTransitive() { return parent_transitive; }

StmtInfoPtrSet PKB::ParentRelation::getReverseTransitive() { return children_transitive; }

void PKB::ParentRelation::optimize(StatementRelationStore<ParentRelation>& store) {
	for (auto& item : store.map) {
		if (item.second.parent == nullptr) {
			ParentRelation::populateTransitive(store, item.second, {});
		}
	}
}

StmtInfoPtrSet PKB::ParentRelation::populateTransitive(StatementRelationStore<ParentRelation>& store,
                                                                            ParentRelation& current,
                                                                            StmtInfoPtrSet previous) {
	current.appendForwardTransitive(previous);
	previous.insert(current.self);
	StmtInfoPtrSet result;
	for (const shared_ptr<StmtInfo>& child : current.children) {
		auto relation = store.map.find(child->getIdentifier());
		StmtInfoPtrSet transitive_children = populateTransitive(store, relation->second, previous);
		result.insert(transitive_children.begin(), transitive_children.end());
	}
	current.appendReverseTransitive(result);
	result.insert(current.self);
	return result;
}
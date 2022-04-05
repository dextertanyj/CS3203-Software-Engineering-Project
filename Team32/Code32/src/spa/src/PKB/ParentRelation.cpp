#include "PKB/ParentRelation.h"

#include <stdexcept>
#include <utility>

void PKB::ParentRelation::insertForward(const shared_ptr<StmtInfo>& parent_to_insert) {
	assert(getSelf()->getIdentifier() > parent_to_insert->getIdentifier());
	assert(this->parent == nullptr);

	this->parent = parent_to_insert;
}

void PKB::ParentRelation::insertReverse(const shared_ptr<StmtInfo>& child_to_insert) {
	assert(getSelf()->getIdentifier() < child_to_insert->getIdentifier());
	this->children.emplace(child_to_insert);
}

StmtInfoPtrSet PKB::ParentRelation::getForward() const {
	if (parent == nullptr) {
		return {};
	}
	return {parent};
}

StmtInfoPtrSet PKB::ParentRelation::getReverse() const { return children; }

// Template specializations for Parent relationship.

template <>
void PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::ParentRelation>::optimize() {
	for (auto& item : map) {
		if (item.second.getForward().empty()) {
			populateTransitive(item.second, {});
		}
	}
}

template <>
StmtInfoPtrSet PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::ParentRelation>::populateTransitive(ParentRelation& current,
                                                                                                        StmtInfoPtrSet previous) {
	current.appendForwardTransitive(previous);
	previous.emplace(current.getSelf());
	StmtInfoPtrSet result;
	for (const shared_ptr<StmtInfo>& child : current.getReverse()) {
		auto relation = map.find(child->getIdentifier());
		StmtInfoPtrSet transitive_children = populateTransitive(relation->second, previous);
		result.insert(transitive_children.begin(), transitive_children.end());
	}
	current.appendReverseTransitive(result);
	result.emplace(current.getSelf());
	return result;
}
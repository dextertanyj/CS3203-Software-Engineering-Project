#include "PKB/CallRelation.h"

#include "PKB/TransitiveRelationStore.h"

PKB::CallRelation::CallRelation(shared_ptr<ProcedureInfo> self) : self(std::move(self)) {}

void PKB::CallRelation::insertForward(const shared_ptr<ProcedureInfo>& caller) { this->callers.insert(caller); }

void PKB::CallRelation::insertReverse(const shared_ptr<ProcedureInfo>& callee) { this->callees.insert(callee); }

void PKB::CallRelation::appendForwardTransitive(unordered_set<shared_ptr<ProcedureInfo>> new_callers) {
	this->callers_transitive.insert(new_callers.begin(), new_callers.end());
}

void PKB::CallRelation::appendReverseTransitive(unordered_set<shared_ptr<ProcedureInfo>> new_callees) {
	this->callees_transitive.insert(new_callees.begin(), new_callees.end());
}

shared_ptr<PKB::ProcedureInfo> PKB::CallRelation::getSelf() const { return self; }

unordered_set<shared_ptr<PKB::ProcedureInfo>> PKB::CallRelation::getForward() const { return callers; }

unordered_set<shared_ptr<PKB::ProcedureInfo>> PKB::CallRelation::getReverse() const { return callees; }

unordered_set<shared_ptr<PKB::ProcedureInfo>> PKB::CallRelation::getForwardTransitive() const { return callers_transitive; }

unordered_set<shared_ptr<PKB::ProcedureInfo>> PKB::CallRelation::getReverseTransitive() const { return callees_transitive; }

// Template specializations for Call relationship.

template <>
unordered_set<shared_ptr<PKB::ProcedureInfo>>
PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation>::populateTransitive(
	CallRelation& current, unordered_set<shared_ptr<ProcedureInfo>> previous) {
	current.appendForwardTransitive(previous);
	previous.insert(current.getSelf());
	unordered_set<shared_ptr<ProcedureInfo>> result;
	unordered_set<shared_ptr<ProcedureInfo>> callers;
	for (const shared_ptr<ProcedureInfo>& callee : current.getReverse()) {
		if (callers.find(callee) != callers.end()) {
			throw logic_error("Recursive call detected.");
		}
		auto relation = map.find(callee->getIdentifier());
		unordered_set<shared_ptr<ProcedureInfo>> transitive_callees = populateTransitive(relation->second, previous);
		result.insert(transitive_callees.begin(), transitive_callees.end());
	}
	current.appendReverseTransitive(result);
	result.insert(current.getSelf());
	return result;
}

template <>
void PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation>::optimize() {
	for (auto& item : map) {
		if (item.second.getForward().empty()) {
			populateTransitive(item.second, {});
		}
	}
}
#include "PKB/CallRelation.h"

#include <stdexcept>

#include "PKB/TransitiveRelationStore.h"

PKB::CallRelation::CallRelation(shared_ptr<ProcedureInfo> self) : self(std::move(self)) {}

void PKB::CallRelation::insertForward(const shared_ptr<ProcedureInfo>& caller) {
	if (self == caller) {
		throw invalid_argument("Recursive call detected.");
	}
	this->callers.insert(caller);
}

void PKB::CallRelation::insertReverse(const shared_ptr<ProcedureInfo>& callee) {
	if (self == callee) {
		throw invalid_argument("Recursive call detected.");
	}
	this->callees.insert(callee); }

void PKB::CallRelation::appendForwardTransitive(unordered_set<shared_ptr<ProcedureInfo>> new_callers) {
	if (new_callers.find(self) != new_callers.end()) {
		throw invalid_argument("Recursive call detected.");
	}
	this->callers_transitive.insert(new_callers.begin(), new_callers.end());
}

void PKB::CallRelation::appendReverseTransitive(unordered_set<shared_ptr<ProcedureInfo>> new_callees) {
	if (new_callees.find(self) != new_callees.end()) {
		throw invalid_argument("Recursive call detected.");
	}
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
	unordered_set<shared_ptr<ProcedureInfo>> result = current.getReverseTransitive();
	bool unset = result.empty();
	unordered_set<shared_ptr<ProcedureInfo>> callers = current.getForwardTransitive();
	for (const shared_ptr<ProcedureInfo>& callee : current.getReverse()) {
		if (callers.find(callee) != callers.end()) {
			throw logic_error("Recursive call detected.");
		}
		auto relation = map.find(callee->getIdentifier());
		unordered_set<shared_ptr<ProcedureInfo>> transitive_callees = populateTransitive(relation->second, previous);
		// If the node has been visited before, we optimize by skipping the duplicate addition of children.
		if (unset) {
			result.insert(transitive_callees.begin(), transitive_callees.end());
		}
	}
	if (unset) {
		current.appendReverseTransitive(result);
	}
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
#include "PKB/CallsRelation.h"

#include <stdexcept>

void PKB::CallsRelation::insertForward(const shared_ptr<ProcedureInfo>& caller) {
	if (getSelf() == caller) {
		throw invalid_argument("Recursive call detected.");
	}
	this->callers.emplace(caller);
}

void PKB::CallsRelation::insertReverse(const shared_ptr<ProcedureInfo>& callee) {
	if (getSelf() == callee) {
		throw invalid_argument("Recursive call detected.");
	}
	this->callees.emplace(callee);
}

void PKB::CallsRelation::appendForwardTransitive(const unordered_set<shared_ptr<ProcedureInfo>>& new_callers) {
	if (new_callers.find(getSelf()) != new_callers.end()) {
		throw invalid_argument("Recursive call detected.");
	}
	TransitiveRelation<ProcedureInfo>::appendForwardTransitive(new_callers);
}

void PKB::CallsRelation::appendReverseTransitive(const unordered_set<shared_ptr<ProcedureInfo>>& new_callees) {
	if (new_callees.find(getSelf()) != new_callees.end()) {
		throw invalid_argument("Recursive call detected.");
	}
	TransitiveRelation<ProcedureInfo>::appendReverseTransitive(new_callees);
}

unordered_set<shared_ptr<PKB::ProcedureInfo>> PKB::CallsRelation::getForward() const { return callers; }

unordered_set<shared_ptr<PKB::ProcedureInfo>> PKB::CallsRelation::getReverse() const { return callees; }

// Template specializations for Call relationship.

template <>
void PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallsRelation>::optimize() {
	for (auto& item : map) {
		if (item.second.getForward().empty()) {
			populateTransitive(item.second, {});
		}
	}
}

template <>
unordered_set<shared_ptr<PKB::ProcedureInfo>>
PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallsRelation>::populateTransitive(
	CallsRelation& current, unordered_set<shared_ptr<ProcedureInfo>> previous) {
	current.appendForwardTransitive(previous);
	previous.emplace(current.getSelf());
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
	result.emplace(current.getSelf());
	return result;
}
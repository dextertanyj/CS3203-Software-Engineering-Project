#include "NodeRelation.h"

#include <stdexcept>

PKB::NodeRelation::NodeRelation(shared_ptr<StmtInfo> self) : self(std::move(self)), is_last_node(false) {}

void PKB::NodeRelation::insertForward(const shared_ptr<StmtInfo>& previous_node) {
	if (self == previous_node) {
		throw invalid_argument("Recursive call detected.");
	}
	if (previous_nodes.size() == 2) {
		throw logic_error("A node cannot have more than 2 previous nodes.");
	}
	this->previous_nodes.insert(previous_node);
}

void PKB::NodeRelation::insertReverse(const shared_ptr<StmtInfo>& next_node) {
	if (self == next_node) {
		throw invalid_argument("Recursive call detected.");
	}
	if (next_nodes.size() == 2) {
		throw logic_error("A node cannot have more than 2 next nodes.");
	}
	StmtType current_type = self->getType();
	if (current_type != StmtType::IfStmt && current_type != StmtType::WhileStmt && next_nodes.size() == 1) {
		throw logic_error("Non-conditional nodes cannot have more than 1 next nodes.");
	}
	this->next_nodes.insert(next_node);
}

void PKB::NodeRelation::appendForwardTransitive(StmtInfoPtrSet new_previous_nodes) {
	if (new_previous_nodes.find(self) != new_previous_nodes.end()) {
		throw invalid_argument("Recursive call detected.");
	}
	this->previous_nodes_transitive.insert(new_previous_nodes.begin(), new_previous_nodes.end());
}

void PKB::NodeRelation::appendReverseTransitive(StmtInfoPtrSet new_callees) {
	if (new_callees.find(self) != new_callees.end()) {
		throw invalid_argument("Recursive call detected.");
	}
	this->next_nodes_transitive.insert(new_callees.begin(), new_callees.end());
}

shared_ptr<StmtInfo> PKB::NodeRelation::getSelf() const { return self; }

StmtInfoPtrSet PKB::NodeRelation::getForward() const { return previous_nodes; }

StmtInfoPtrSet PKB::NodeRelation::getReverse() const { return next_nodes; }

StmtInfoPtrSet PKB::NodeRelation::getForwardTransitive() const { return previous_nodes_transitive; }

StmtInfoPtrSet PKB::NodeRelation::getReverseTransitive() const { return next_nodes_transitive; }

void PKB::NodeRelation::setLastNode() { this->is_last_node = true; }

void PKB::NodeRelation::setCFGIndex(StmtRef ref) { this->cfg_index = ref; }

// Template specializations for Call relationship.

template <>
StmtInfoPtrSet PKB::TransitiveRelationStore<ProcRef, StmtInfo, PKB::NodeRelation>::populateTransitive(
	NodeRelation& current, StmtInfoPtrSet previous) {
	/* TODO: Implement Next* here
	current.appendForwardTransitive(previous);
	previous.insert(current.getSelf());
	StmtInfoPtrSet result = current.getReverseTransitive();
	bool unset = result.empty();
	StmtInfoPtrSet callers = current.getForwardTransitive();
	for (const shared_ptr<StmtInfo>& callee : current.getReverse()) {
	    if (callers.find(callee) != callers.end()) {
	        throw logic_error("Recursive call detected.");
	    }
	    auto relation = map.find(callee->getIdentifier());
	    StmtInfoPtrSet transitive_callees = populateTransitive(relation->second, previous);
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
 */
	return {};
}

template <>
void PKB::TransitiveRelationStore<ProcRef, StmtInfo, PKB::NodeRelation>::optimize() {
	/* TODO: Implement Next* here
	for (auto& item : map) {
	    if (item.second.getForward().empty()) {
	        populateTransitive(item.second, {});
	    }
	}
	*/
}
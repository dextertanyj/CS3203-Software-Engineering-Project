#include "NodeRelation.h"

#include <queue>
#include <stdexcept>

#include "NodeComparator.h"
#include "NodeInfo.h"

PKB::NodeRelation::NodeRelation(shared_ptr<PKB::NodeInfo> self) : self(std::move(self)) {}

void PKB::NodeRelation::insertForward(const shared_ptr<PKB::NodeInfo>& previous_node) {
	if (self == previous_node) {
		throw invalid_argument("Recursive call detected.");
	}
	if (previous_nodes.size() == 2) {
		throw logic_error("A node cannot have more than 2 previous nodes.");
	}
	this->previous_nodes.insert(previous_node);
}

void PKB::NodeRelation::insertReverse(const shared_ptr<PKB::NodeInfo>& next_node) {
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

void PKB::NodeRelation::appendForwardTransitive(unordered_set<shared_ptr<NodeInfo>> new_previous_nodes) {
	if (new_previous_nodes.find(self) != new_previous_nodes.end()) {
		throw invalid_argument("Recursive call detected.");
	}
	this->previous_nodes_transitive.insert(new_previous_nodes.begin(), new_previous_nodes.end());
}

void PKB::NodeRelation::appendReverseTransitive(unordered_set<shared_ptr<NodeInfo>> new_next_nodes) {
	if (new_next_nodes.find(self) != new_next_nodes.end()) {
		throw invalid_argument("Recursive call detected.");
	}
	this->next_nodes_transitive.insert(new_next_nodes.begin(), new_next_nodes.end());
}

shared_ptr<PKB::NodeInfo> PKB::NodeRelation::getSelf() const { return self; }

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NodeRelation::getForward() const { return previous_nodes; }

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NodeRelation::getReverse() const { return next_nodes; }

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NodeRelation::getForwardTransitive() const { return previous_nodes_transitive; }

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NodeRelation::getReverseTransitive() const { return next_nodes_transitive; }

void PKB::NodeRelation::setCFGIndex(StmtRef ref) { this->self->setUniqueIndex(ref); }

// Template specializations for CFG Next relationship.

template <>
unordered_set<shared_ptr<PKB::NodeInfo>> PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>::populateTransitive(
	NodeRelation& current, unordered_set<shared_ptr<PKB::NodeInfo>> next_nodes_transitive) {
	current.appendReverseTransitive(next_nodes_transitive);
	next_nodes_transitive.insert(current.getSelf());
	return next_nodes_transitive;
}

template <>
void PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>::optimize() {
	NodeRelation last_node = NodeRelation(nullptr);
	for (auto& item : map) {
		// Start optimization from the end of the CFG.
		if (item.second.getReverse().empty()) {
			last_node = item.second;
			break;
		}
	}
	// Use last node's stmt ref as the unique index of a CFG.
	StmtRef unique_index = last_node.getSelf()->getIdentifier();
	unordered_set<shared_ptr<PKB::NodeInfo>> next_nodes_transitive = {};

	// PQ stores a pair of stmt ref and the transitive next nodes to add to that ref's node.
	// populateTransitive will then perform the insertion into the NodeRelation, while returning
	// a new set of transitive next nodes to add to subsequent parent nodes.
	std::priority_queue<pair<StmtRef, unordered_set<shared_ptr<PKB::NodeInfo>>>,
	                    vector<pair<StmtRef, unordered_set<shared_ptr<PKB::NodeInfo>>>>, NodeComparator>
		queue;
	queue.push(make_pair(last_node.getSelf()->getIdentifier(), next_nodes_transitive));
	while (!queue.empty()) {
		StmtRef current_ref = queue.top().first;
		NodeRelation current_node_relation = map.find(current_ref)->second;
		unordered_set<shared_ptr<NodeInfo>> nodes_to_add = queue.top().second;
		current_node_relation.setCFGIndex(unique_index);
		nodes_to_add = populateTransitive(current_node_relation, nodes_to_add);
		for (shared_ptr<PKB::NodeInfo> previous_node : current_node_relation.getForward()) {
			queue.push(make_pair(previous_node->getIdentifier(), nodes_to_add));
		}
		queue.pop();
	}
}
#include "NodeRelation.h"

#include "PKB/TransitiveRelationStore.tpp"

PKB::NodeRelation::NodeRelation(shared_ptr<PKB::NodeInfo> self) : self(std::move(self)) {}

void PKB::NodeRelation::insertForward(const shared_ptr<PKB::NodeInfo>& previous_node) {
	if (self == previous_node) {
		throw invalid_argument("Recursive call detected.");
	}
	if (previous_nodes.size() == 3) {
		throw logic_error("A node cannot have more than 3 previous nodes.");
	}
	this->previous_nodes.insert(previous_node);
}

void PKB::NodeRelation::insertReverse(const shared_ptr<PKB::NodeInfo>& next_node) {
	if (self == next_node) {
		throw invalid_argument("Recursive call detected.");
	}
	if (next_nodes.size() == 3) {
		throw logic_error("A node cannot have more than 3 next nodes.");
	}
	StmtType current_type = self->getType();
	if (current_type != StmtType::IfStmt && current_type != StmtType::WhileStmt && next_nodes.size() == 1) {
		throw logic_error("Non-conditional nodes cannot have more than 1 next nodes.");
	}
	this->next_nodes.insert(next_node);
}

void PKB::NodeRelation::appendForwardTransitive(unordered_set<shared_ptr<NodeInfo>> new_previous_nodes) {
	this->previous_nodes_transitive.insert(new_previous_nodes.begin(), new_previous_nodes.end());
}

void PKB::NodeRelation::appendReverseTransitive(unordered_set<shared_ptr<NodeInfo>> new_next_nodes) {
	this->next_nodes_transitive.insert(new_next_nodes.begin(), new_next_nodes.end());
}

shared_ptr<PKB::NodeInfo> PKB::NodeRelation::getSelf() const { return self; }

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NodeRelation::getForward() const { return previous_nodes; }

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NodeRelation::getReverse() const { return next_nodes; }

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NodeRelation::getForwardTransitive() const { return previous_nodes_transitive; }

unordered_set<shared_ptr<PKB::NodeInfo>> PKB::NodeRelation::getReverseTransitive() const { return next_nodes_transitive; }

void PKB::NodeRelation::setCFGIndex(StmtRef ref) { this->self->setUniqueIndex(ref); }

void PKB::NodeRelation::removeForward(shared_ptr<NodeInfo> node_info) { previous_nodes.erase(node_info); }

void PKB::NodeRelation::removeReverse(shared_ptr<NodeInfo> node_info) { next_nodes.erase(node_info); }

shared_ptr<PKB::NodeInfo> PKB::NodeRelation::findLastNode(shared_ptr<NodeInfo> node_ref,
                                                          TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>& store) {
	shared_ptr<NodeInfo> current = node_ref;
	unordered_set<shared_ptr<NodeInfo>> next_nodes = store.getReverse(current->getIdentifier());
	while (!next_nodes.empty()) {
		// Chosen branch should not matter.
		current = shared_ptr<NodeInfo>(store.getReverse(current->getIdentifier()).begin()->get());
		next_nodes = store.getReverse(current->getIdentifier());
	}
	return current;
}

void PKB::NodeRelation::populateUniqueIndex(const NodeRelation& node_relation, StmtRef stmt_no,
                                            PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>& store) {}

/* Template specializations for CFG Next relationship.
 populateTransitive method is not required, as this population will be done
 per query and cannot be pre-computed.
 */
template <>
void PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>::optimize() {
	std::set<int> if_stmts;
	// Find all If nodes and add them into an ordered set
	for (auto& item : map) {
		NodeRelation relation = item.second;
		if (relation.getSelf()->getType() == StmtType::IfStmt) {
			if_stmts.insert(relation.getSelf()->getIdentifier());
		}
	}

	for (auto iter = if_stmts.rbegin(); iter != if_stmts.rend(); ++iter) {
		unordered_set<shared_ptr<NodeInfo>> direct_nodes = this->getReverse(*iter);
		vector<shared_ptr<NodeInfo>> sorted_direct_nodes;
		// Sort direct_next_nodes
		std::copy(direct_nodes.begin(), direct_nodes.end(), sorted_direct_nodes.begin());
		std::sort(sorted_direct_nodes.begin(), sorted_direct_nodes.end());
		shared_ptr<NodeInfo> larger_direct_node = sorted_direct_nodes[1];
		shared_ptr<PKB::NodeInfo> larger_direct_node_end = PKB::NodeRelation::findLastNode(larger_direct_node, *this);
		shared_ptr<PKB::NodeInfo> smaller_direct_node_end = map.at(larger_direct_node.get()->getIdentifier() - 1).getSelf();

		// If node only has two direct next nodes, add dummy node before exiting if block
		if (direct_nodes.size() == 2) {
			// Create dummy node and set these 2 ending nodes to the dummy node.
			PKB::NodeInfo dummy_node = PKB::DummyNodeInfo();
			shared_ptr<PKB::NodeInfo> dummy_node_ptr = make_shared<PKB::NodeInfo>(dummy_node);
			this->set(larger_direct_node_end, dummy_node_ptr);
			this->set(smaller_direct_node_end, dummy_node_ptr);
		}

		// Pre-processing: Connect ending statements within if/else blocks to the following statement of the if control statement.
		if (direct_nodes.size() == 3) {
			shared_ptr<NodeInfo> following_node = sorted_direct_nodes[2];
			this->set(larger_direct_node_end, following_node);
			this->set(smaller_direct_node_end, following_node);
			// Remove link between if control node and following node
			NodeRelation current_if_node = map.at(*iter);
			current_if_node.removeReverse(following_node);
			map.at(following_node->getIdentifier()).removeForward(current_if_node.getSelf());
		}
	}
	// Populate unique index after pre-processing is done.
	for (auto& item : map) {
		if (item.second.getReverse().empty()) {
			PKB::NodeRelation::populateUniqueIndex(item.second, item.second.getSelf()->getIdentifier(), *this);
		}
	}
}

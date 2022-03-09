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

/* Template specializations for CFG Next relationship.
 populateTransitive method is not required, as this population will be done
 per query and cannot be pre-computed.
 */
template <>
void PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>::optimize() {
	// Edge case: If statement is final statement in the procedure. if and else blocks have no ending node.
	std::set<int> edge_case_if_stmts;
	for (auto& item : map) {
		NodeRelation relation = item.second;
		// Edge case detected when if control statement only has 2 direct next statements.
		// Gather these nodes in a set and add dummy nodes starting from the highest StmtRef.
		if (relation.getSelf()->getType() == StmtType::IfStmt && relation.getReverse().size() == 2) {
			edge_case_if_stmts.insert(relation.getSelf()->getIdentifier());
		}
	}
	// Start from the highest StmtRef if statement.
	for (auto iter = edge_case_if_stmts.rbegin(); iter != edge_case_if_stmts.rend(); ++iter) {
		unordered_set<shared_ptr<NodeInfo>> child_nodes = this->getReverse(*iter);
		vector<shared_ptr<NodeInfo>> child_nodes_list(child_nodes.size());
		std::copy(child_nodes.begin(), child_nodes.end(), child_nodes_list.begin());
		// Need to use vector to differentiate first and second child node. There should be 2 child nodes.
		shared_ptr<NodeInfo> child_node_1 = *(child_nodes_list.begin());
		shared_ptr<NodeInfo> child_node_2 = *(child_nodes_list.rbegin());
		// Find the last node within the block.
		shared_ptr<PKB::NodeInfo> child_node_1_end_node = PKB::NodeRelation::findLastNode(child_node_1, *this);
		shared_ptr<PKB::NodeInfo> child_node_2_end_node = PKB::NodeRelation::findLastNode(child_node_2, *this);
		// Create dummy node and set these 2 ending nodes to the dummy node.
		PKB::NodeInfo dummy_node = PKB::DummyNodeInfo();
		shared_ptr<PKB::NodeInfo> dummy_node_ptr = make_shared<PKB::NodeInfo>(dummy_node);
		this->set(child_node_1_end_node, dummy_node_ptr);
		this->set(child_node_2_end_node, dummy_node_ptr);
	}

	// Pre-processing: Connect ending statements within if/else blocks to the following statement of the if control statement.

	// Populate unique index after pre-processing is done.
}
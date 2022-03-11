#ifndef SPA_NODEINTERFACE_H
#define SPA_NODEINTERFACE_H

#include <algorithm>
#include <vector>

#include "PKB/PKB.h"

class PKB::NodeInterface {
public:
	void setUniqueIndex(size_t index) {
		if (this->unique_index != 0) {
			throw invalid_argument("Node has already been assigned a unique CFG index.");
		}
		this->unique_index = index;
	}
	size_t getUniqueIndex() { return this->unique_index; }
	string getNodeType() { return this->node_type; }

	void insertNext(shared_ptr<PKB::NodeInterface> next) {
		if (any_of(next_nodes.begin(), next_nodes.end(), next)) {
			throw invalid_argument("Node to be inserted is already present.");
		}
		this->next_nodes.push_back(next);
	}

	void insertPrevious(shared_ptr<PKB::NodeInterface> prev) {
		if (any_of(previous_nodes.begin(), previous_nodes.end(), prev)) {
			throw invalid_argument("Node to be inserted is already present.");
		}
		this->previous_nodes.push_back(prev);
	}

	void removeNext(shared_ptr<PKB::NodeInterface> to_remove) {
		auto iter = std::find(next_nodes.begin(), next_nodes.end(), to_remove);
		if (iter == next_nodes.end()) {
			throw invalid_argument("Node to remove does not exist as next of this node.");
		}
		this->next_nodes.erase(iter);
	};

	void removePrevious(shared_ptr<NodeInterface> to_remove) {
		auto iter = std::find(previous_nodes.begin(), previous_nodes.end(), to_remove);
		if (iter == previous_nodes.end()) {
			throw invalid_argument("Node to remove does not exist as previous of this node.");
		}
		this->previous_nodes.erase(iter);
	};

	vector<shared_ptr<PKB::NodeInterface>> getPreviousNodes() { return this->previous_nodes; };

	vector<shared_ptr<PKB::NodeInterface>> getNextNodes() { return this->next_nodes; };

	size_t getNodeRef() const { return this->node_ref; };

	virtual shared_ptr<PKB::NodeInterface> getDummyNode() = 0;
	virtual void setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) = 0;

	// Comparator for priority queue
	friend bool operator<(PKB::NodeInterface lhs, PKB::NodeInterface rhs) { return lhs.getNodeRef() < rhs.getNodeRef(); };
	friend bool operator>(PKB::NodeInterface lhs, PKB::NodeInterface rhs) { return lhs.getNodeRef() > rhs.getNodeRef(); };

protected:
	bool is_last_node;
	size_t unique_index = 0;  // Default to 0 for convenience when populating unique index later.
	size_t node_ref;
	string node_type;
	vector<shared_ptr<PKB::NodeInterface>> previous_nodes;
	vector<shared_ptr<PKB::NodeInterface>> next_nodes;
	// Transitive store not required as we are not allowed to precompute.
};

#endif  // SPA_NODEINTERFACE_H

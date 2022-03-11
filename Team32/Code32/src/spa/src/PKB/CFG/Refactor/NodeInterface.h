#ifndef SPA_NODEINTERFACE_H
#define SPA_NODEINTERFACE_H

#include <algorithm>
#include <set>

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
		this->next_nodes.insert(next);
	}

	void insertPrevious(shared_ptr<PKB::NodeInterface> prev) {
		if (any_of(previous_nodes.begin(), previous_nodes.end(), prev)) {
			throw invalid_argument("Node to be inserted is already present.");
		}
		this->previous_nodes.insert(prev);
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

	set<shared_ptr<PKB::NodeInterface>> getPreviousNodes() { return this->previous_nodes; };

	set<shared_ptr<PKB::NodeInterface>> getNextNodes() { return this->next_nodes; };

	virtual size_t getNodeRef() { throw logic_error("getNodeRef should not be called in base class"); }

	virtual shared_ptr<PKB::NodeInterface> getDummyNode() { throw logic_error("getDummyNode should not be called in base class"); };

	virtual void setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) {
		throw logic_error("setDummyNode should not be called in base class");
	};

	// Comparator for priority queue
	friend bool operator<(PKB::NodeInterface lhs, PKB::NodeInterface rhs) { return lhs.getNodeRef() < rhs.getNodeRef(); };
	friend bool operator>(PKB::NodeInterface lhs, PKB::NodeInterface rhs) { return lhs.getNodeRef() > rhs.getNodeRef(); };

protected:
	size_t unique_index = 0;  // Default to 0 for convenience when populating unique index later.
	string node_type;
	set<shared_ptr<PKB::NodeInterface>> previous_nodes;
	set<shared_ptr<PKB::NodeInterface>> next_nodes;
	// Transitive store not required as we are not allowed to precompute.
};

#endif  // SPA_NODEINTERFACE_H

#ifndef SPA_NODEINTERFACE_H
#define SPA_NODEINTERFACE_H

#include <algorithm>
#include <set>

#include "PKB/PKB.h"

class PKB::NodeInterface {
public:
	void insertNext(shared_ptr<PKB::NodeInterface> next) { this->next_nodes.insert(next); }
	void insertPrevious(shared_ptr<PKB::NodeInterface> prev) { this->previous_nodes.insert(prev); }
	set<shared_ptr<PKB::NodeInterface>> getPreviousNodes() { return this->previous_nodes; };
	set<shared_ptr<PKB::NodeInterface>> getNextNodes() { return this->next_nodes; };
	size_t getGraphIndex() { return this->graph_index; }
	void setGraphIndex(size_t index) {
		if (this->graph_index != 0) {
			throw invalid_argument("Node has already been assigned a unique graph index.");
		}
		this->graph_index = index;
	}
	NodeType getNodeType() { return this->node_type; }

	virtual size_t getNodeRef() { throw logic_error("getNodeRef should not be called in base class"); }
	virtual ~NodeInterface() = default;

protected:
	size_t graph_index = 0;  // Default to 0 for convenience when populating graph index later.
	NodeType node_type;
	set<shared_ptr<PKB::NodeInterface>> previous_nodes;
	set<shared_ptr<PKB::NodeInterface>> next_nodes;
};

#endif  // SPA_NODEINTERFACE_H

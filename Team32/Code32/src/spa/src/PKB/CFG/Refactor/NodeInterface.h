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
	size_t getUniqueIndex() { return this->unique_index; }
	void setUniqueIndex(size_t index) {
		if (this->unique_index != 0) {
			throw invalid_argument("Node has already been assigned a unique CFG index.");
		}
		this->unique_index = index;
	}
	string getNodeType() { return this->node_type; }

	virtual size_t getNodeRef() { throw logic_error("getNodeRef should not be called in base class"); }
	virtual shared_ptr<PKB::NodeInterface> getDummyNode() { throw logic_error("getDummyNode should not be called in base class"); };
	virtual void setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) {
		throw logic_error("setDummyNode should not be called in base class");
	};
	virtual ~NodeInterface(){};

protected:
	size_t unique_index = 0;  // Default to 0 for convenience when populating unique index later.
	string node_type;
	set<shared_ptr<PKB::NodeInterface>> previous_nodes;
	set<shared_ptr<PKB::NodeInterface>> next_nodes;
};

#endif  // SPA_NODEINTERFACE_H

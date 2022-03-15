#include "PKB/CFG/NodeInterface.h"

PKB::NodeInterface::NodeInterface(NodeType type) : node_type(type) {}

void PKB::NodeInterface::setGraphIndex(size_t index) {
	if (this->graph_index != 0) {
		throw invalid_argument("Node has already been assigned a unique graph index.");
	}
	graph_index = index;
}

void PKB::NodeInterface::insertNext(shared_ptr<NodeInterface> next) { next_nodes.insert(next); }

void PKB::NodeInterface::insertPrevious(shared_ptr<NodeInterface> previous) { previous_nodes.insert(previous); }

set<shared_ptr<PKB::NodeInterface>> PKB::NodeInterface::getNextNodes() const { return next_nodes; }

set<shared_ptr<PKB::NodeInterface>> PKB::NodeInterface::getPreviousNodes() const { return previous_nodes; }

size_t PKB::NodeInterface::getGraphIndex() const { return this->graph_index; }

PKB::NodeType PKB::NodeInterface::getNodeType() const { return this->node_type; }
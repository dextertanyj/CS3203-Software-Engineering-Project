#include "PKB/CFG/NodeInterface.h"

#include <algorithm>
#include <cassert>
#include <cstddef>

using namespace std;

PKB::NodeInterface::NodeInterface(Types::NodeType type) : node_type(type) {}

void PKB::NodeInterface::setGraphIndex(size_t index) {
	assert(graph_index == 0);
	graph_index = index;
}

void PKB::NodeInterface::insertNext(const shared_ptr<NodeInterface>& next) { next_nodes.push_back(next); }

void PKB::NodeInterface::insertPrevious(const shared_ptr<NodeInterface>& prev) { previous_nodes.push_back(prev); }

unordered_set<shared_ptr<PKB::NodeInterface>> PKB::NodeInterface::getNextNodes() const {
	unordered_set<shared_ptr<NodeInterface>> results;
	for (const auto& ptr : next_nodes) {
		results.emplace(ptr.lock());
	}
	return results;
}

unordered_set<shared_ptr<PKB::NodeInterface>> PKB::NodeInterface::getPreviousNodes() const {
	unordered_set<shared_ptr<NodeInterface>> results;
	for (const auto& ptr : previous_nodes) {
		results.emplace(ptr.lock());
	}
	return results;
}

size_t PKB::NodeInterface::getGraphIndex() const { return graph_index; }

PKB::Types::NodeType PKB::NodeInterface::getNodeType() const { return node_type; }
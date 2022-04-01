#include "PKB/CFG/NodeInterface.h"

PKB::NodeInterface::NodeInterface(NodeType type) : node_type(type) {}

void PKB::NodeInterface::setGraphIndex(size_t index) {
    if (this->graph_index != 0) {
        throw invalid_argument("Node has already been assigned a unique graph index.");
    }
    graph_index = index;
}

void PKB::NodeInterface::insertNext(shared_ptr<NodeInterface> next) { next_nodes.push_back(next); }

void PKB::NodeInterface::insertPrevious(shared_ptr<NodeInterface> previous) { previous_nodes.push_back(previous); }

unordered_set<shared_ptr<PKB::NodeInterface>> PKB::NodeInterface::getNextNodes() const {
    unordered_set<shared_ptr<NodeInterface>> results;
    for (const auto &ptr: next_nodes) {
        results.insert(ptr.lock());
    }
    return results;
}

unordered_set<shared_ptr<PKB::NodeInterface>> PKB::NodeInterface::getPreviousNodes() const {
    unordered_set<shared_ptr<NodeInterface>> results;
    for (const auto &ptr: previous_nodes) {
        results.insert(ptr.lock());
    }
    return results;
}

size_t PKB::NodeInterface::getGraphIndex() const { return this->graph_index; }

PKB::NodeType PKB::NodeInterface::getNodeType() const { return this->node_type; }
#include "DummyNode.h"

PKB::DummyNode::DummyNode(size_t ref) {
	this->if_control_stmt_ref = ref;
	this->node_type = NodeType::Dummy;
}

size_t PKB::DummyNode::getNodeRef() { return this->if_control_stmt_ref; }

void PKB::DummyNode::insertNext(shared_ptr<PKB::NodeInterface> next) {
	if (this->next_nodes.size() == 1) {
		throw logic_error("Dummy node cannot have more than 1 next statement.");
	}
	this->next_nodes.insert(next);
}

void PKB::DummyNode::insertPrevious(shared_ptr<PKB::NodeInterface> prev) { this->previous_nodes.insert(prev); }

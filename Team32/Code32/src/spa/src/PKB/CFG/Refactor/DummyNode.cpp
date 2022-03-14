#include "DummyNode.h"

PKB::DummyNode::DummyNode(size_t ref) {
	this->if_control_stmt_ref = ref;
	this->node_type = NodeType::Dummy;
}

size_t PKB::DummyNode::getNodeRef() { return this->if_control_stmt_ref; }

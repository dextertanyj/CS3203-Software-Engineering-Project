#include "DummyNode.h"

PKB::DummyNode::DummyNode(size_t ref) {
	this->if_control_stmt_ref = ref;
	this->node_type = NodeType::Dummy;
}
shared_ptr<PKB::NodeInterface> PKB::DummyNode::getDummyNode() { throw logic_error("Dummy node should not call getDummyNode."); }

void PKB::DummyNode::setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) {
	throw logic_error("Dummy node should not call setDummyNode.");
}
size_t PKB::DummyNode::getNodeRef() { return this->if_control_stmt_ref; }

#include "DummyNode.h"

PKB::DummyNode::DummyNode(size_t ref) {
	this->node_ref = ref;
	this->node_type = "dummy";
}
shared_ptr<PKB::NodeInterface> PKB::DummyNode::getDummyNode() {
	throw logic_error("Dummy node should not call getDummyNode.");
}

void PKB::DummyNode::setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) {
	throw logic_error("Dummy node should not call setDummyNode.");
}

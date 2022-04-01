#include "DummyNode.h"

PKB::DummyNode::DummyNode(StmtRef ref) : PKB::NodeInterface(NodeType::Dummy), if_control_stmt_ref(ref) {}

size_t PKB::DummyNode::getNodeRef() const { return this->if_control_stmt_ref; }

void PKB::DummyNode::setConnection(shared_ptr<PKB::NodeInterface> next) {
	setNext(next);
	next->setPrevious(shared_from_this());
}

void PKB::DummyNode::setNext(shared_ptr<PKB::NodeInterface> next) {
	if (!getNextNodes().empty()) {
		throw logic_error("Dummy node cannot have more than 1 next statement.");
	}
	insertNext(next);
}

void PKB::DummyNode::setPrevious(shared_ptr<PKB::NodeInterface> previous) {
	if (getPreviousNodes().size() > 1) {
		throw logic_error("Dummy node cannot have more than 1 next statement.");
	}
	insertPrevious(previous);
}
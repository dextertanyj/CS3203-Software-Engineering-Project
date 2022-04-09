#include "PKB/CFG/DummyNode.h"

#include <cassert>

#include "PKB/Types.h"

using namespace std;

PKB::CFG::DummyNode::DummyNode(StmtRef index) : NodeInterface(Types::NodeType::Dummy), if_control_stmt_ref(index) {}

size_t PKB::CFG::DummyNode::getNodeRef() const { return if_control_stmt_ref; }

void PKB::CFG::DummyNode::setConnection(shared_ptr<NodeInterface> next) {
	setNext(next);
	next->setPrevious(shared_from_this());
}

void PKB::CFG::DummyNode::setNext(const shared_ptr<NodeInterface>& next) {
	assert(getNextNodes().empty());
	insertNext(next);
}

void PKB::CFG::DummyNode::setPrevious(const shared_ptr<NodeInterface>& previous) {
	assert(getPreviousNodes().size() < 2);
	insertPrevious(previous);
}
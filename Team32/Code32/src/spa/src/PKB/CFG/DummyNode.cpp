#include "PKB/CFG/DummyNode.h"

#include <cassert>

#include "PKB/Types.h"

using namespace std;

PKB::DummyNode::DummyNode(StmtRef ref) : PKB::NodeInterface(Types::NodeType::Dummy), if_control_stmt_ref(ref) {}

size_t PKB::DummyNode::getNodeRef() const { return if_control_stmt_ref; }

void PKB::DummyNode::setConnection(shared_ptr<PKB::NodeInterface> next) {
	setNext(next);
	next->setPrevious(shared_from_this());
}

void PKB::DummyNode::setNext(const shared_ptr<PKB::NodeInterface>& next) {
	assert(getNextNodes().empty());
	insertNext(next);
}

void PKB::DummyNode::setPrevious(const shared_ptr<PKB::NodeInterface>& previous) {
	assert(getPreviousNodes().size() < 2);
	insertPrevious(previous);
}
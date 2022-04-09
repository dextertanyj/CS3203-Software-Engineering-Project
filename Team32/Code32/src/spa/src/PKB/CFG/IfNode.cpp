#include "PKB/CFG/IfNode.h"

#include <cassert>

#include "PKB/Types.h"

using namespace std;

PKB::IfNode::IfNode(const StmtInfoPtr& info)
	: StatementNode(Types::NodeType::If, info), dummy_node(make_shared<DummyNode>(info->getIdentifier())) {
	assert(info->getType() == StmtType::If);
}

void PKB::IfNode::setConnection(shared_ptr<NodeInterface> next) {
	dummy_node->setNext(next);
	next->setPrevious(dummy_node);
}

void PKB::IfNode::setNext(const shared_ptr<NodeInterface>& next) {
	assert(getNextNodes().size() < 2);
	insertNext(next);
}

void PKB::IfNode::setPrevious(const shared_ptr<NodeInterface>& prev) {
	assert(getPreviousNodes().empty());
	insertPrevious(prev);
}

void PKB::IfNode::insertIfNext(const shared_ptr<StatementNode>& first, const shared_ptr<StatementNode>& second) {
	assert(getNextNodes().empty());
	setNext(first);
	setNext(second);
	first->setPrevious(shared_from_this());
	second->setPrevious(shared_from_this());
}

void PKB::IfNode::insertIfExit(const shared_ptr<StatementNode>& first, const shared_ptr<StatementNode>& second) {
	first->setConnection(dummy_node);
	second->setConnection(dummy_node);
}

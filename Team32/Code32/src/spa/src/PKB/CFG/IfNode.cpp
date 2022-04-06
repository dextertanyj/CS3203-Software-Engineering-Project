#include "PKB/CFG/IfNode.h"

#include <cassert>

#include "PKB/CFG/DummyNode.h"
#include "PKB/Types.h"

using namespace std;

PKB::IfNode::IfNode(const StmtInfoPtr& info)
	: PKB::StatementNode(Types::NodeType::If, info), dummy_node(make_shared<PKB::DummyNode>(info->getIdentifier())) {
	assert(info->getType() == StmtType::If);
}

void PKB::IfNode::setConnection(shared_ptr<PKB::NodeInterface> next) {
	dummy_node->setNext(next);
	next->setPrevious(dummy_node);
}

void PKB::IfNode::setNext(const shared_ptr<PKB::NodeInterface>& next) {
	assert(getNextNodes().size() < 2);
	insertNext(next);
}

void PKB::IfNode::setPrevious(const shared_ptr<PKB::NodeInterface>& previous) {
	assert(getPreviousNodes().empty());
	insertPrevious(previous);
}

void PKB::IfNode::insertIfNext(const shared_ptr<PKB::StatementNode>& first, const shared_ptr<PKB::StatementNode>& second) {
	assert(getNextNodes().empty());
	setNext(first);
	setNext(second);
	first->setPrevious(shared_from_this());
	second->setPrevious(shared_from_this());
}

void PKB::IfNode::insertIfExit(const shared_ptr<PKB::StatementNode>& first, const shared_ptr<PKB::StatementNode>& second) {
	first->setConnection(dummy_node);
	second->setConnection(dummy_node);
}

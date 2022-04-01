#include "PKB/CFG/IfNode.h"

#include "PKB/CFG/DummyNode.h"

PKB::IfNode::IfNode(const shared_ptr<StmtInfo>& info)
	: PKB::StatementNode(NodeType::If, info), dummy_node(make_shared<PKB::DummyNode>(info->getIdentifier())) {
	if (info->getType() != StmtType::IfStmt) {
		throw invalid_argument("Provided statement info is not an if statement");
	}
}

void PKB::IfNode::setConnection(shared_ptr<PKB::NodeInterface> next) {
	dummy_node->setNext(next);
	next->setPrevious(dummy_node);
}

void PKB::IfNode::setNext(const shared_ptr<PKB::NodeInterface>& next) { insertNext(next); }

void PKB::IfNode::setPrevious(const shared_ptr<PKB::NodeInterface>& prev) {
	if (!getPreviousNodes().empty()) {
		throw logic_error("Non-dummy node cannot have more than 1 previous node.");
	}
	insertPrevious(prev);
}

void PKB::IfNode::insertIfNext(const shared_ptr<PKB::StatementNode>& first, const shared_ptr<PKB::StatementNode>& second) {
	setNext(first);
	setNext(second);
	first->setPrevious(shared_from_this());
	second->setPrevious(shared_from_this());
}

void PKB::IfNode::insertIfExit(const shared_ptr<PKB::StatementNode>& first, const shared_ptr<PKB::StatementNode>& second) {
	first->setConnection(dummy_node);
	second->setConnection(dummy_node);
}

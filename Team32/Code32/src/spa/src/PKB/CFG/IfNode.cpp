#include "IfNode.h"

#include "DummyNode.h"

PKB::IfNode::IfNode(shared_ptr<StmtInfo> info) : PKB::StatementNode(info) {
	if (info->getType() != StmtType::IfStmt) {
		throw invalid_argument("Provided statement info is not an if statement");
	}
	this->node_type = NodeType::If;
	this->dummy_node = make_shared<PKB::DummyNode>(PKB::DummyNode(info->getIdentifier()));
}
shared_ptr<PKB::NodeInterface> PKB::IfNode::getDummyNode() { return this->dummy_node; }

size_t PKB::IfNode::getNodeRef() { return this->stmt_info->getIdentifier(); }

void PKB::IfNode::insertNext(shared_ptr<PKB::NodeInterface> next) { this->dummy_node->insertNext(next); }

void PKB::IfNode::insertPrevious(shared_ptr<PKB::NodeInterface> prev) {
	if (this->previous_nodes.size() == 1) {
		throw logic_error("Non-dummy node cannot have more than 1 previous node.");
	}
	this->previous_nodes.insert(prev);
}
void PKB::IfNode::insertIfNext(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second, shared_ptr<PKB::IfNode> self) {
	if (this->next_nodes.size() == 2) {
		throw logic_error("If's children nodes have already been inserted.");
	}
	this->next_nodes.insert(first);
	this->next_nodes.insert(second);
	first->insertPrevious(self);
	second->insertPrevious(self);
}

void PKB::IfNode::insertIfExit(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second) {
	if (this->dummy_node->getPreviousNodes().size() == 2) {
		throw logic_error("If's dummy node's previous nodes have already been inserted.");
	}
	if (first->getNodeType() == NodeType::If) {
		shared_ptr<PKB::IfNode> first_if = dynamic_pointer_cast<IfNode>(first);
		this->dummy_node->insertPrevious(first_if->getDummyNode());
	} else {
		this->dummy_node->insertPrevious(first);
	}
	if (second->getNodeType() == NodeType::If) {
		shared_ptr<PKB::IfNode> second_if = dynamic_pointer_cast<IfNode>(second);
		this->dummy_node->insertPrevious(second_if->getDummyNode());
	} else {
		this->dummy_node->insertPrevious(second);
	}
	first->insertNext(this->dummy_node);
	second->insertNext(this->dummy_node);
}

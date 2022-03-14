#include "IfNode.h"

PKB::IfNode::IfNode(shared_ptr<StmtInfo> info) {
	if (info->getType() != StmtType::IfStmt) {
		throw invalid_argument("Provided statement info is not an if statement");
	}
	this->statement_info = info;
	this->node_type = NodeType::If;
}
shared_ptr<PKB::NodeInterface> PKB::IfNode::getDummyNode() { return this->dummy_node; }

void PKB::IfNode::setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) { this->dummy_node = to_insert; }

size_t PKB::IfNode::getNodeRef() { return this->statement_info->getIdentifier(); }

void PKB::IfNode::insertNext(shared_ptr<PKB::NodeInterface> next) { this->dummy_node->insertNext(next); }

void PKB::IfNode::insertPrevious(shared_ptr<PKB::NodeInterface> prev, bool to_dummy) {
	if (this->previous_nodes.size() == 1) {
		throw logic_error("Non-dummy node cannot have more than 1 previous node.");
	}
	if (prev->getNodeType() == NodeType::If && to_dummy) {
		shared_ptr<IfNode> prev_if = dynamic_pointer_cast<IfNode>(prev);
		this->previous_nodes.insert(prev_if->getDummyNode());
	} else {
		this->previous_nodes.insert(prev);
	}
}
void PKB::IfNode::insertIfNext(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second, shared_ptr<PKB::IfNode> self) {
	if (this->next_nodes.size() == 2) {
		throw logic_error("If's children nodes have already been inserted.");
	}
	this->next_nodes.insert(first);
	this->next_nodes.insert(second);
	first->insertPrevious(self, false);
	second->insertPrevious(self, false);
}

void PKB::IfNode::insertIfExit(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second) {
	if (this->dummy_node->getPreviousNodes().size() == 2) {
		throw logic_error("If's dummy node's previous nodes have already been inserted.");
	}
	this->dummy_node->insertPrevious(first);
	this->dummy_node->insertPrevious(second);
	first->insertNext(this->dummy_node);
	second->insertNext(this->dummy_node);
}

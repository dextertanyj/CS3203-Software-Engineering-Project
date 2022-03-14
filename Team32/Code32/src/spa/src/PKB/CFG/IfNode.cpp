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

void PKB::IfNode::insertNext(shared_ptr<PKB::NodeInterface> next) {
	if (this->next_nodes.size() == 2) {
		throw logic_error("If node cannot have more than 2 next statements.");
	}
	this->next_nodes.insert(next);
}

void PKB::IfNode::insertPrevious(shared_ptr<PKB::NodeInterface> prev) {
	if (this->previous_nodes.size() == 1) {
		throw logic_error("Non-dummy node cannot have more than 1 previous node.");
	}
	this->previous_nodes.insert(prev);
}

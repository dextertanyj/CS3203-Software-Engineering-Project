#include "NonConditionalNode.h"

#include "IfNode.h"

PKB::NonConditionalNode::NonConditionalNode(shared_ptr<StmtInfo> info) {
	if (info->getType() == StmtType::IfStmt || info->getType() == StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info cannot be a conditional statement");
	}
	this->statement_info = info;
	this->node_type = NodeType::NonConditional;
}

size_t PKB::NonConditionalNode::getNodeRef() { return this->statement_info->getIdentifier(); }

void PKB::NonConditionalNode::insertNext(shared_ptr<PKB::NodeInterface> next) {
	if (this->next_nodes.size() == 1) {
		throw logic_error("Non-conditional node cannot have more than 1 next statement.");
	}
	this->next_nodes.insert(next);
}

void PKB::NonConditionalNode::insertPrevious(shared_ptr<PKB::NodeInterface> prev, bool to_dummy) {
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

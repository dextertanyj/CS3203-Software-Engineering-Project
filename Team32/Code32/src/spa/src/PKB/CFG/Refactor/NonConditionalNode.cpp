#include "NonConditionalNode.h"

PKB::NonConditionalNode::NonConditionalNode(shared_ptr<StmtInfo> info) {
	if (info->getType() == StmtType::IfStmt || info->getType() == StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info cannot be a conditional statement");
	}
	this->statement_info = info;
	this->node_type = "non_conditional";
}

void PKB::NonConditionalNode::setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) {
	throw logic_error("Non conditional node should not call setDummyNode.");
}

shared_ptr<PKB::NodeInterface> PKB::NonConditionalNode::getDummyNode() {
	throw logic_error("Non conditional node should not call getDummyNode.");
}
size_t PKB::NonConditionalNode::getNodeRef() { return this->statement_info->getIdentifier(); }

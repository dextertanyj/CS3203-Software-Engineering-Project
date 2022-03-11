#include "IfNode.h"

PKB::IfNode::IfNode(shared_ptr<StmtInfo> info) {
	if (info->getType() != StmtType::IfStmt) {
		throw invalid_argument("Provided statement info is not an if statement");
	}
	this->statement_info = info;
	this->node_type = "if";
}
shared_ptr<PKB::NodeInterface> PKB::IfNode::getDummyNode() { return this->dummy_node; }

void PKB::IfNode::setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) { this->dummy_node = to_insert; }

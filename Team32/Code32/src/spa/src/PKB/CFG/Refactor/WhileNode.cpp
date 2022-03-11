#include "WhileNode.h"

PKB::WhileNode::WhileNode(shared_ptr<StmtInfo> info) {
	if (info->getType() != StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info is not a while statement");
	}
	this->statement_info = info;
	this->node_type = "while";
}

shared_ptr<PKB::NodeInterface> PKB::WhileNode::getDummyNode() { throw logic_error("While node should not call getDummyNode."); }

void PKB::WhileNode::setDummyNode(shared_ptr<PKB::NodeInterface> to_insert) {
	throw logic_error("While node should not call setDummyNode.");
}

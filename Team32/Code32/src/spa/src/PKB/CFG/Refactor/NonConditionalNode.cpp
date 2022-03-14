#include "NonConditionalNode.h"

PKB::NonConditionalNode::NonConditionalNode(shared_ptr<StmtInfo> info) {
	if (info->getType() == StmtType::IfStmt || info->getType() == StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info cannot be a conditional statement");
	}
	this->statement_info = info;
	this->node_type = NodeType::NonConditional;
}

size_t PKB::NonConditionalNode::getNodeRef() { return this->statement_info->getIdentifier(); }

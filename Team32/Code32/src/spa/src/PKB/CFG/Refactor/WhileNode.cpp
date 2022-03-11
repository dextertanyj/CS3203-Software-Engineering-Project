#include "WhileNode.h"

PKB::WhileNode::WhileNode(shared_ptr<StmtInfo> info) {
	if (info->getType() != StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info is not a while statement");
	}
	this->statement_info = info;
	this->node_type = "while";
}

#include "WhileNode.h"

PKB::WhileNode::WhileNode(shared_ptr<StmtInfo> info) {
	if (info->getType() != StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info is not a while statement");
	}
	this->statement_info = info;
	this->node_type = NodeType::While;
}

size_t PKB::WhileNode::getNodeRef() { return this->statement_info->getIdentifier(); }

void PKB::WhileNode::insertNext(shared_ptr<PKB::NodeInterface> next) {
	if (this->next_nodes.size() == 2) {
		throw logic_error("While Node cannot have more than 2 next statements.");
	}
	this->next_nodes.insert(next);
}
void PKB::WhileNode::insertPrevious(shared_ptr<PKB::NodeInterface> prev) {
	if (this->previous_nodes.size() == 2) {
		throw logic_error("While node cannot have more than 2 previous nodes.");
	}
	this->previous_nodes.insert(prev);
}

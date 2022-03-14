#include "WhileNode.h"

#include "IfNode.h"

PKB::WhileNode::WhileNode(shared_ptr<StmtInfo> info) : PKB::StatementNode(info) {
	if (info->getType() != StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info is not a while statement");
	}
	this->node_type = NodeType::While;
}

size_t PKB::WhileNode::getNodeRef() { return this->stmt_info->getIdentifier(); }

void PKB::WhileNode::insertNext(shared_ptr<PKB::NodeInterface> next) {
	if (this->next_nodes.size() == 2) {
		throw logic_error("While Node cannot have more than 2 next statements.");
	}
	this->next_nodes.insert(next);
}
void PKB::WhileNode::insertPrevious(shared_ptr<PKB::NodeInterface> prev, bool to_dummy) {
	if (this->previous_nodes.size() == 2) {
		throw logic_error("While node cannot have more than 2 previous nodes.");
	}
	if (prev->getNodeType() == NodeType::If && to_dummy) {
		shared_ptr<IfNode> prev_if = dynamic_pointer_cast<IfNode>(prev);
		this->previous_nodes.insert(prev_if->getDummyNode());
	} else {
		this->previous_nodes.insert(prev);
	}
}

#include "PKB/CFG/NonConditionalNode.h"

PKB::NonConditionalNode::NonConditionalNode(shared_ptr<StmtInfo> info) : PKB::StatementNode(NodeType::NonConditional, info) {
	if (info->getType() == StmtType::IfStmt || info->getType() == StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info cannot be a conditional statement");
	}
}

void PKB::NonConditionalNode::setNext(shared_ptr<NodeInterface> next) {
	if (getNextNodes().size() == 1) {
		throw logic_error("Non-conditional node cannot have more than 1 next statement.");
	}
	this->insertNext(next);
}

void PKB::NonConditionalNode::setPrevious(shared_ptr<NodeInterface> prev) {
	if (getPreviousNodes().size() == 1) {
		throw logic_error("Non-dummy node cannot have more than 1 previous node.");
	}
	this->insertPrevious(prev);
}

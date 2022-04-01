#include "PKB/CFG/WhileNode.h"

PKB::WhileNode::WhileNode(const shared_ptr<StmtInfo>& info) : PKB::StatementNode(NodeType::While, info) {
	if (info->getType() != StmtType::WhileStmt) {
		throw invalid_argument("Provided statement info is not a while statement");
	}
}

void PKB::WhileNode::setNext(shared_ptr<NodeInterface> next) {
	if (getNextNodes().size() > 1) {
		throw logic_error("While Node cannot have more than 2 next statements.");
	}
	insertNext(next);
}

void PKB::WhileNode::setPrevious(shared_ptr<NodeInterface> prev) {
	if (getPreviousNodes().size() > 1) {
		throw logic_error("While node cannot have more than 2 previous nodes.");
	}
	insertPrevious(prev);
}

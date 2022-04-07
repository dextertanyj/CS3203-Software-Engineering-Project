#include "PKB/CFG/WhileNode.h"

#include <cassert>

#include "PKB/Types.h"

PKB::WhileNode::WhileNode(const shared_ptr<StmtInfo>& info) : PKB::StatementNode(Types::NodeType::While, info) {
	assert(info->getType() == StmtType::While);
}

void PKB::WhileNode::setNext(const shared_ptr<NodeInterface>& next) {
	assert(getNextNodes().size() < 2);
	insertNext(next);
}

void PKB::WhileNode::setPrevious(const shared_ptr<NodeInterface>& prev) {
	assert(getPreviousNodes().size() < 2);
	insertPrevious(prev);
}

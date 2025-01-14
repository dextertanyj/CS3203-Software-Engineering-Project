#include "PKB/CFG/WhileNode.h"

#include <cassert>

#include "PKB/Types.h"

using namespace std;

PKB::CFG::WhileNode::WhileNode(const StmtInfoPtr& info) : StatementNode(Types::NodeType::While, info) {
	assert(info->getType() == StmtType::While);
}

void PKB::CFG::WhileNode::setNext(const shared_ptr<NodeInterface>& next) {
	assert(getNextNodes().size() < 2);
	insertNext(next);
}

void PKB::CFG::WhileNode::setPrevious(const shared_ptr<NodeInterface>& prev) {
	assert(getPreviousNodes().size() < 2);
	insertPrevious(prev);
}

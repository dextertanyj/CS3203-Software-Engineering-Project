#include "PKB/CFG/StatementNode.h"

#include <cassert>

using namespace std;

PKB::StatementNode::StatementNode(const StmtInfoPtr& info) : NodeInterface(Types::NodeType::Statement), stmt_info(info) {
	assert(info->getType() != StmtType::While || info->getType() != StmtType::If);
}

PKB::StatementNode::StatementNode(Types::NodeType type, const StmtInfoPtr& info) : NodeInterface(type), stmt_info(info) {}

void PKB::StatementNode::setConnection(shared_ptr<NodeInterface> next) {
	setNext(next);
	next->setPrevious(shared_from_this());
}

void PKB::StatementNode::setNext(const shared_ptr<NodeInterface>& next) {
	assert(getNextNodes().empty());
	insertNext(next);
}

void PKB::StatementNode::setPrevious(const shared_ptr<NodeInterface>& prev) {
	assert(getPreviousNodes().empty());
	insertPrevious(prev);
}

StmtInfoPtr PKB::StatementNode::getStmtInfo() const { return stmt_info; }

StmtRef PKB::StatementNode::getNodeRef() const { return stmt_info->getIdentifier(); }

#include "PKB/CFG/StatementNode.h"

#include <cassert>

PKB::StatementNode::StatementNode(const shared_ptr<StmtInfo>& info) : NodeInterface(NodeType::NonConditional), stmt_info(info) {
	assert(info->getType() != StmtType::WhileStmt || info->getType() != StmtType::IfStmt);
}

PKB::StatementNode::StatementNode(NodeType type, const shared_ptr<StmtInfo>& info) : NodeInterface(type), stmt_info(info) {}

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

shared_ptr<StmtInfo> PKB::StatementNode::getStmtInfo() const { return stmt_info; }

StmtRef PKB::StatementNode::getNodeRef() const { return stmt_info->getIdentifier(); }

bool PKB::StatementNode::operator<(const PKB::StatementNode& other) const { return getNodeRef() < other.getNodeRef(); }

bool PKB::StatementNode::operator>(const PKB::StatementNode& other) const { return getNodeRef() > other.getNodeRef(); }

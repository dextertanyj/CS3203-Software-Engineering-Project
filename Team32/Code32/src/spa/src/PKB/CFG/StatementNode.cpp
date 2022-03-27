#include "PKB/CFG/StatementNode.h"

#include <utility>

PKB::StatementNode::StatementNode(NodeType type, shared_ptr<StmtInfo> info) : NodeInterface(type), stmt_info(info) {}

void PKB::StatementNode::setConnection(shared_ptr<NodeInterface> next) {
	setNext(next);
	next->setPrevious(shared_from_this());
}

StmtRef PKB::StatementNode::getNodeRef() const { return stmt_info->getIdentifier(); }

bool PKB::StatementNode::operator<(const PKB::StatementNode& other) const { return this->getNodeRef() < other.getNodeRef(); }

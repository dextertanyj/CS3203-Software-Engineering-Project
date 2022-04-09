#include "PKB/CFG/StatementNode.h"

#include <cassert>
#include <utility>

using namespace std;

PKB::CFG::StatementNode::StatementNode(const StmtInfoPtr& info) : NodeInterface(Types::NodeType::Statement), stmt_info(info) {
	assert(info->getType() != StmtType::While || info->getType() != StmtType::If);
}

PKB::CFG::StatementNode::StatementNode(Types::NodeType type, StmtInfoPtr info) : NodeInterface(type), stmt_info(move(info)) {}

void PKB::CFG::StatementNode::setConnection(shared_ptr<NodeInterface> next) {
	setNext(next);
	next->setPrevious(shared_from_this());
}

void PKB::CFG::StatementNode::setNext(const shared_ptr<NodeInterface>& next) {
	assert(getNextNodes().empty());
	insertNext(next);
}

void PKB::CFG::StatementNode::setPrevious(const shared_ptr<NodeInterface>& prev) {
	assert(getPreviousNodes().empty());
	insertPrevious(prev);
}

StmtInfoPtr PKB::CFG::StatementNode::getStmtInfo() const { return stmt_info; }

StmtRef PKB::CFG::StatementNode::getNodeRef() const { return stmt_info->getIdentifier(); }

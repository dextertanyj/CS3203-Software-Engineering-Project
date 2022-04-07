#include "SP/Node/StatementListNode.h"

#include <cassert>
#include <utility>

using namespace std;

void SP::Node::StatementListNode::addStatementNode(unique_ptr<StatementNode> statement) { stmt_list.push_back(move(statement)); }

vector<StmtRef> SP::Node::StatementListNode::extract(PKB::StorageUpdateInterface& pkb) const {
	vector<StmtRef> children;
	for (auto iter = stmt_list.begin(); iter < stmt_list.end(); ++iter) {
		children.push_back(iter->get()->extract(pkb));
	}
	assert(!children.empty());
	StmtRef previous = children.at(0);
	for (auto iter = ++children.begin(); iter < children.end(); ++iter) {
		pkb.setFollows(previous, *iter);
		pkb.setNext(previous, *iter);
		previous = *iter;
	}
	return children;
}

bool SP::Node::StatementListNode::equals(const shared_ptr<StatementListNode>& object) const {
	if (this->stmt_list.size() != object->stmt_list.size()) {
		return false;
	}
	for (size_t i = 0; i < this->stmt_list.size(); i++) {
		if (!this->stmt_list[i]->equals(object->stmt_list[i])) {
			return false;
		}
	}
	return true;
}

vector<shared_ptr<SP::Node::StatementNode>> SP::Node::StatementListNode::getStatementList() const { return this->stmt_list; }

unique_ptr<SP::Node::StatementListNode> SP::Node::StatementListNode::parseStatementList(Lexer& lex, StmtRef& statement_count) {
	unique_ptr<StatementListNode> statement_list = make_unique<StatementListNode>();
	do {
		statement_list->addStatementNode(StatementNode::parseStatement(lex, statement_count));
	} while (lex.peekToken() != CLOSE_BRACES);
	return statement_list;
}

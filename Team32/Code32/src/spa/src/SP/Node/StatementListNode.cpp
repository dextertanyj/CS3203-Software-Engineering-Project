#include "SP/Node/StatementListNode.h"

#include <cassert>

using namespace std;

SP::Node::StatementListNode::StatementListNode() = default;

void SP::Node::StatementListNode::addStatementNode(unique_ptr<StatementNode> statement) { stmt_list.push_back(move(statement)); }

unique_ptr<SP::Node::StatementListNode> SP::Node::StatementListNode::parseStatementList(Lexer& lex, StmtRef& statement_count) {
	unique_ptr<StatementListNode> statement_list = make_unique<StatementListNode>();
	do {
		statement_list->addStatementNode(StatementNode::parseStatement(lex, statement_count));
	} while (lex.peekToken() != "}");
	return statement_list;
}

vector<StmtRef> SP::Node::StatementListNode::extract(PKB::StorageUpdateInterface& pkb) {
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

bool SP::Node::StatementListNode::equals(const shared_ptr<StatementListNode>& object) {
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

vector<shared_ptr<SP::Node::StatementNode>> SP::Node::StatementListNode::getStatementList() { return this->stmt_list; }

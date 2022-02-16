#include "SP/Node/StatementListNode.h"

using namespace std;

SP::Node::StatementListNode::StatementListNode() = default;

void SP::Node::StatementListNode::addStatementNode(unique_ptr<StatementNode> statement) { stmtList.push_back(move(statement)); }

unique_ptr<SP::Node::StatementListNode> SP::Node::StatementListNode::parseStatementList(Lexer& lex, int& statement_count) {
	unique_ptr<StatementListNode> statement_list = make_unique<StatementListNode>();
	do {
		statement_list->addStatementNode(StatementNode::parseStatement(lex, statement_count));
	} while (lex.peekToken() != "}");
	return statement_list;
}

vector<StmtRef> SP::Node::StatementListNode::extract(PKB& pkb) {
	vector<StmtRef> children;
	for (auto iter = stmtList.begin(); iter < stmtList.end(); ++iter) {
		children.push_back(iter->get()->extract(pkb));
	}
	if (children.empty()) {
		throw ParseException("Statement list is empty");
	}
	StmtRef previous = children.at(0);
	for (auto iter = ++children.begin(); iter < children.end(); ++iter) {
		pkb.setFollows(previous, *iter);
		previous = *iter;
	}
	return children;
}
bool SP::Node::StatementListNode::equals(const shared_ptr<StatementListNode>& object) {
	if (this->stmtList.size() != object->stmtList.size()) {
		return false;
	}
	for (int i = 0; i < this->stmtList.size(); i++) {
		if (!this->stmtList[i]->equals(object->stmtList[i])) {
			return false;
		}
	}
	return true;
}

vector<shared_ptr<SP::Node::StatementNode>> SP::Node::StatementListNode::getStatementList() { return this->stmtList; }

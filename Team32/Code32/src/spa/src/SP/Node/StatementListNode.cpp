#include "SP/Node/StatementListNode.h"
#include <memory>
#include <vector>
#include <iostream>
#include <type_traits>

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

StmtInfoList SP::Node::StatementListNode::extract(PKB& pkb) {
	StmtInfoList children;
	for (auto iter = stmtList.begin(); iter < stmtList.end(); ++iter) {
		children.push_back(std::make_shared<StmtInfo>(iter->get()->extract(pkb)));
	}
	StmtInfo previous = *(children.at(0));
	for (auto iter = ++children.begin(); iter < children.end(); ++iter) {
		pkb.setFollows(previous.reference, iter->get()->reference);
		previous = *(iter->get());
	}
	return children;
}
bool SP::Node::StatementListNode::equals(shared_ptr<StatementListNode> object) {
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

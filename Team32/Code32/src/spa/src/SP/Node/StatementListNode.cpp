#include "SP/Node/StatementListNode.h"

#include <vector>

using namespace std;

StatementListNode::StatementListNode() = default;

void StatementListNode::addStatementNode(unique_ptr<StatementNode> statement) { stmtList.push_back(move(statement)); }

unique_ptr<StatementListNode> StatementListNode::parseStatementList(Lexer& lex, int& statement_count) {
	unique_ptr<StatementListNode> statement_list = make_unique<StatementListNode>();
	do {
		statement_list->addStatementNode(StatementNode::parseStatement(lex, statement_count));
	} while (lex.peekToken() != "}");
	return statement_list;
}

StmtInfoList StatementListNode::extract(PKB& pkb) {
	StmtInfoList children;
	for (auto iter = stmtList.begin(); iter < stmtList.end(); ++iter) {
		children.push_back(iter->get()->extract(pkb));
	}
	StmtInfo previous = children.at(0);
	for (auto iter = ++children.begin(); iter < children.end(); ++iter) {
		pkb.setFollows(previous.reference, iter->reference);
		previous = *iter;
	}
	return children;
}
bool StatementListNode::equals(shared_ptr<StatementListNode> object) {
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

vector<shared_ptr<StatementNode>> StatementListNode::getStatementList() {
    return this->stmtList;
}
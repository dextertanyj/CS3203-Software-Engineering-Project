#include <vector>

#include "SP/StatementListNode.h"

using namespace std;

StatementListNode::StatementListNode() = default;

void StatementListNode::addStatementNode(unique_ptr<StatementNode> statement) {
    stmtList.push_back(move(statement));
}

#include "StatementListNode.h"
#include <vector>

using namespace std;

StatementListNode::StatementListNode() = default;

void StatementListNode::addStatementNode(StatementNode stmtNode) {
    stmtList.push_back(make_unique<StatementNode>(stmtNode));
}

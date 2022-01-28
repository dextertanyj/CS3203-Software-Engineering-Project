#include "StatementListNode.h"
#include <vector>

using namespace std;

StatementListNode::StatementListNode() = default;

void StatementListNode::addStatementNode(unique_ptr<StatementNode> stmtNode) {
    stmtList.push_back(move(stmtNode));
}

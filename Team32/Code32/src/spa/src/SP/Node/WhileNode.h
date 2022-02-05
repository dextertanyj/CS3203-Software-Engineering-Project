#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include <string>

#include "SP/Node/ConditionalExpressionNode.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/StatementListNode.h"

using namespace std;

class WhileNode : public StatementNode {
public:
    WhileNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> stmtLst);
private:
    unique_ptr<ConditionalExpressionNode> condExpr;
    unique_ptr<StatementListNode> stmtLst;
};

#endif //SPA_WHILENODE_H

#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include <string>

#include "SP/ConditionalExpressionNode.h"
#include "SP/StatementNode.h"
#include "SP/StatementListNode.h"

using namespace std;

class WhileNode : public StatementNode {
public:
    WhileNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> stmtLst);
private:
    unique_ptr<ConditionalExpressionNode> condExpr;
    unique_ptr<StatementListNode> stmtLst;
};

#endif //SPA_WHILENODE_H

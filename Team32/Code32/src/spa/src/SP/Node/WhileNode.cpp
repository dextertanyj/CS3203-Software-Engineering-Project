#include "WhileNode.h"

WhileNode::WhileNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> stmtLst):
        StatementNode(stmtNo),
        condExpr(move(condExpr)),
        stmtLst(move(stmtLst)){
}

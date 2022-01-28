#include "SP/IfNode.h"

using namespace std;

IfNode::IfNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> ifStmtLst
               , unique_ptr<StatementListNode> elseStmtLst):
        StatementNode(stmtNo),
        condExpr(move(condExpr)),
        ifStmtLst(move(ifStmtLst)),
        elseStmtLst(move(elseStmtLst)){
}

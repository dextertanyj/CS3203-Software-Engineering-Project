#include "IfNode.h"

IfNode::IfNode(StmtRef stmtNo, CondExpr condExpr, StatementListNode ifStmtLst, StatementListNode elseStmtLst):
        StatementNode(stmtNo),
        condExpr(condExpr),
        ifStmtLst(ifStmtLst),
        elseStmtLst(elseStmtLst){
}

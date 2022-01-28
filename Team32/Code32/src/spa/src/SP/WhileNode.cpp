#include "WhileNode.h"

WhileNode::WhileNode(StmtRef stmtNo, CondExpr condExpr, StatementListNode stmtLst):
        StatementNode(stmtNo),
        condExpr(condExpr),
        stmtLst(stmtLst){
}

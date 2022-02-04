#include "Parent.h"

Parent::Parent(bool isStar, QueryStmtRef parentStmt, QueryStmtRef childStmt)
	: isStar(isStar),
	  parentStmt(parentStmt),
	  childStmt(childStmt) {}

QueryStmtRef Parent::getParentStmt() {
	return parentStmt;
}

QueryStmtRef Parent::getChildStmt() {
	return childStmt;
}

bool Parent::getIsStar() {
	return isStar;
}

QueryResult Parent::execute() {
	return QueryResult();
}



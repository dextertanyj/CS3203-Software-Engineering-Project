#include "Follows.h"

Follows::Follows(bool isStar, QueryStmtRef leftStmt, QueryStmtRef rightStmt)
	: isStar(isStar),
	  leftStmt(leftStmt),
	  rightStmt(rightStmt) {}

QueryStmtRef Follows::getLeftStmt() {
	return leftStmt;
}

QueryStmtRef Follows::getRightStmt() {
	return rightStmt;
}

bool Follows::getIsStar() {
	return isStar;
}

bool Follows::isTrivialCase() {
	return false;
}

bool Follows::executeTrivial(PKB& pkb) {
	return false;
}

QueryResult Follows::executeNonTrivial(PKB& pkb) {
	return QueryResult();
}

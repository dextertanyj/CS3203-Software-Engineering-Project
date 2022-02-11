#include "QP/Relationship/Follows.h"

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

QueryResult Follows::execute(PKB& pkb, QueryResult& result) {
	return QueryResult();
}

vector<string> Follows::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->leftStmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->leftStmt.stmtRef);
	}
	if (this->rightStmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->rightStmt.stmtRef);
	}
	return declarationSymbols;
}

bool Follows::isTrivialCase() {
	return false;
}

QueryResult Follows::executeTrivial(PKB& pkb) {
	return QueryResult();
}

QueryResult Follows::executeNonTrivial(PKB& pkb, QueryResult& result) {
	return QueryResult();
}

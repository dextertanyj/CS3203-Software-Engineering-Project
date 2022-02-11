#include "QP/Relationship/Parent.h"

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

QueryResult Parent::execute(PKB& pkb, QueryResult& result) {
	return isTrivialCase() ? executeTrivial(pkb) : executeNonTrivial(pkb, result);
}

vector<string> Parent::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->parentStmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->parentStmt.stmtRef);
	}
	if (this->childStmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->childStmt.stmtRef);
	}
	return declarationSymbols;
}

bool Parent::isTrivialCase() {
	return this->parentStmt.type != StmtRefType::synonym && this->childStmt.type != StmtRefType::synonym;
}

QueryResult Parent::executeTrivial(PKB& pkb) {
	return QueryResult();
}

QueryResult Parent::executeNonTrivial(PKB& pkb, QueryResult& result) {
	return QueryResult();
}

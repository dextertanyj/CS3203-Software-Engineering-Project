#include "ModifiesS.h"

ModifiesS::ModifiesS(QueryStmtRef stmt, QueryEntRef ent)
	: stmt(stmt),
	  ent(ent) {}

QueryStmtRef ModifiesS::getStmt() {
	return stmt;
}

QueryEntRef ModifiesS::getEnt() {
	return ent;
}

bool ModifiesS::isTrivialCase() {
	return false;
}

bool ModifiesS::executeTrivial(PKB& pkb) {
	return false;
}

QueryResult ModifiesS::executeNonTrivial(PKB& pkb) {
	return QueryResult();
}

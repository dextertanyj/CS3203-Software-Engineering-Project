#include "UsesS.h"

UsesS::UsesS(QueryStmtRef stmt, QueryEntRef ent)
	: stmt(stmt),
	  ent(ent) {}

QueryStmtRef UsesS::getStmt() {
	return stmt;
}

QueryEntRef UsesS::getEnt() {
	return ent;
}

bool UsesS::isTrivialCase() {
	return false;
}

bool UsesS::executeTrivial(PKB& pkb) {
	return false;
}

QueryResult UsesS::executeNonTrivial(PKB& pkb) {
	return QueryResult();
}

#include "QP/Relationship/UsesS.h"

UsesS::UsesS(QueryStmtRef stmt, QueryEntRef ent)
	: stmt(stmt),
	  ent(ent) {}

QueryStmtRef UsesS::getStmt() {
	return stmt;
}

QueryEntRef UsesS::getEnt() {
	return ent;
}

bool UsesS::execute(PKB& pkb, QueryResult& result) {
	return false;
}

bool UsesS::isTrivialCase() {
	return false;
}

bool UsesS::executeTrivial(PKB& pkb) {
	return false;
}

bool UsesS::executeNonTrivial(PKB& pkb, QueryResult& result) {
	return false;
}

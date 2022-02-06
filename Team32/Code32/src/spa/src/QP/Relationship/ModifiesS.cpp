#include "QP/Relationship/ModifiesS.h"

ModifiesS::ModifiesS(QueryStmtRef stmt, QueryEntRef ent)
	: stmt(stmt),
	  ent(ent) {}

QueryStmtRef ModifiesS::getStmt() {
	return stmt;
}

QueryEntRef ModifiesS::getEnt() {
	return ent;
}

bool ModifiesS::execute(PKB& pkb, QueryResult& result) {
	return false;
}

bool ModifiesS::isTrivialCase() {
	return false;
}

bool ModifiesS::executeTrivial(PKB& pkb) {
	return false;
}

bool ModifiesS::executeNonTrivial(PKB& pkb, QueryResult& result) {
	return false;
}

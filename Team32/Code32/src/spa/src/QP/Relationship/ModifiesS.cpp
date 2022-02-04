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

QueryResult ModifiesS::execute() {
	return QueryResult();
}

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

QueryResult UsesS::execute() {
	return QueryResult();
}

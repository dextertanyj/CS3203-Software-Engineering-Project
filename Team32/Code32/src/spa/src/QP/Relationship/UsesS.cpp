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

QueryResult UsesS::execute(PKB& pkb, QueryResult& result) {
	return QueryResult();
}

vector<string> UsesS::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->stmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->stmt.stmtRef);
	}
	if (this->ent.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->ent.entRef);
	}
	return declarationSymbols;
}

bool UsesS::isTrivialCase() {
	return false;
}

QueryResult UsesS::executeTrivial(PKB& pkb) {
	return QueryResult();
}

QueryResult UsesS::executeNonTrivial(PKB& pkb, QueryResult& result) {
	return QueryResult();
}

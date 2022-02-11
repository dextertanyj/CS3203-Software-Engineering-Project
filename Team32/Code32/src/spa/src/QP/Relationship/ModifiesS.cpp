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

QueryResult ModifiesS::execute(PKB& pkb, QueryResult& result) {
	return QueryResult();
}

vector<string> ModifiesS::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->stmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->stmt.stmtRef);
	}
	if (this->ent.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->ent.entRef);
	}
	return declarationSymbols;
}

bool ModifiesS::isTrivialCase() {
	return false;
}

QueryResult ModifiesS::executeTrivial(PKB& pkb) {
	return QueryResult();
}

QueryResult ModifiesS::executeNonTrivial(PKB& pkb, QueryResult& result) {
	return QueryResult();
}

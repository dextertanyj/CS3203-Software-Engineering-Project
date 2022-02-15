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

QueryResult UsesS::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
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

QueryResult UsesS::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	return QueryResult();
}

QueryResult UsesS::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	return QueryResult();
}

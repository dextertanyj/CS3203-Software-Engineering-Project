#include "QP/Relationship/UsesP.h"

UsesP::UsesP(QueryEntRef leftEnt, QueryEntRef rightEnt)
	: leftEnt(leftEnt),
	  rightEnt(rightEnt) {}

QueryEntRef UsesP::getLeftEnt() {
	return leftEnt;
}

QueryEntRef UsesP::getRightEnt() {
	return rightEnt;
}

QueryResult UsesP::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> UsesP::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->leftEnt.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->leftEnt.entRef);
	}
	if (this->rightEnt.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->rightEnt.entRef);
	}
	return declarationSymbols;
}

QueryResult UsesP::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	return QueryResult();
}

QueryResult UsesP::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	return QueryResult();
}

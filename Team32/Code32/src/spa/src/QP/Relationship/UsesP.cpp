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

QueryResult UsesP::execute(PKB& pkb, bool isTrivial) {
	return isTrivial ? executeTrivial(pkb) : executeNonTrivial(pkb);
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

QueryResult UsesP::executeTrivial(PKB& pkb) {
	return QueryResult();
}

QueryResult UsesP::executeNonTrivial(PKB& pkb) {
	return QueryResult();
}

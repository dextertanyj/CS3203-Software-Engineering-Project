#include "QP/Relationship/ModifiesP.h"

ModifiesP::ModifiesP(QueryEntRef leftEnt, QueryEntRef rightEnt)
	: leftEnt(leftEnt),
	  rightEnt(rightEnt) {}

QueryEntRef ModifiesP::getLeftEnt() {
	return leftEnt;
}

QueryEntRef ModifiesP::getRightEnt() {
	return rightEnt;
}

QueryResult ModifiesP::execute(PKB& pkb, bool isTrivial) {
	return isTrivial ? executeTrivial(pkb) : executeNonTrivial(pkb);
}

vector<string> ModifiesP::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->leftEnt.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->leftEnt.entRef);
	}
	if (this->rightEnt.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->rightEnt.entRef);
	}
	return declarationSymbols;
}

QueryResult ModifiesP::executeTrivial(PKB& pkb) {
	return QueryResult();
}

QueryResult ModifiesP::executeNonTrivial(PKB& pkb) {
	return QueryResult();
}

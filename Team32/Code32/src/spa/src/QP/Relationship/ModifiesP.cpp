#include "ModifiesP.h"

ModifiesP::ModifiesP(QueryEntRef leftEnt, QueryEntRef rightEnt)
	: leftEnt(leftEnt),
	  rightEnt(rightEnt) {}

QueryEntRef ModifiesP::getLeftEnt() {
	return leftEnt;
}

QueryEntRef ModifiesP::getRightEnt() {
	return rightEnt;
}

bool ModifiesP::isTrivialCase() {
	return false;
}

bool ModifiesP::executeTrivial(PKB& pkb) {
	return false;
}

QueryResult ModifiesP::executeNonTrivial(PKB& pkb) {
	return QueryResult();
}

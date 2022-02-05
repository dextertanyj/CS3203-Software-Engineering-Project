#include "UsesP.h"

UsesP::UsesP(QueryEntRef leftEnt, QueryEntRef rightEnt)
	: leftEnt(leftEnt),
	  rightEnt(rightEnt) {}

QueryEntRef UsesP::getLeftEnt() {
	return leftEnt;
}

QueryEntRef UsesP::getRightEnt() {
	return rightEnt;
}

bool UsesP::isTrivialCase() {
	return false;
}

bool UsesP::executeTrivial(PKB& pkb) {
	return false;
}

QueryResult UsesP::executeNonTrivial(PKB& pkb) {
	return QueryResult();
}

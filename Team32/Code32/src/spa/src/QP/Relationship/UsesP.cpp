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

bool UsesP::execute(PKB& pkb, QueryResult& result) {
	return false;
}

bool UsesP::isTrivialCase() {
	return false;
}

bool UsesP::executeTrivial(PKB& pkb) {
	return false;
}

bool UsesP::executeNonTrivial(PKB& pkb, QueryResult& result) {
	return false;
}

#include "QP/ModifiesP.h"

ModifiesP::ModifiesP(QueryEntRef leftEnt, QueryEntRef rightEnt)
	: leftEnt(leftEnt),
	  rightEnt(rightEnt) {}

QueryEntRef ModifiesP::getLeftEnt() {
	return leftEnt;
}

QueryEntRef ModifiesP::getRightEnt() {
	return rightEnt;
}

bool ModifiesP::execute(PKB& pkb, QueryResult& result) {
	return false;
}

bool ModifiesP::isTrivialCase() {
	return false;
}

bool ModifiesP::executeTrivial(PKB& pkb) {
	return false;
}

bool ModifiesP::executeNonTrivial(PKB& pkb, QueryResult& result) {
	return false;
}

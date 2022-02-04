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

QueryResult UsesP::execute() {
	return QueryResult();
}

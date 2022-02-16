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

QueryResult ModifiesP::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
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

QueryResult ModifiesP::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	return QueryResult();
}

QueryResult ModifiesP::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	return QueryResult();
}

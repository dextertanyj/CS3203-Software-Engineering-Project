#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class UsesP : public Relation {
public:
	UsesP(QueryEntRef leftEnt, QueryEntRef rightEnt);

	QueryEntRef getLeftEnt();
	QueryEntRef getRightEnt();

	bool isTrivialCase();
	QueryResult execute(PKB& pkb, QueryResult& result);
	vector<string> getDeclarationSymbols();

private:
	QueryEntRef leftEnt;
	QueryEntRef rightEnt;

	QueryResult executeTrivial(PKB& pkb);
	QueryResult executeNonTrivial(PKB& pkb, QueryResult& result);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_

#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class UsesP : public Relation {
public:
	UsesP(QueryEntRef leftEnt, QueryEntRef rightEnt);

	QueryEntRef getLeftEnt();
	QueryEntRef getRightEnt();

	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);
	vector<string> getDeclarationSymbols();

private:
	QueryEntRef leftEnt;
	QueryEntRef rightEnt;

	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_

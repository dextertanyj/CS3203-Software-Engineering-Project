#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_

#include "Relation.h"
#include "QP/QueryTypeDefs.h"

class UsesP : public Relation {
public:
	UsesP(QueryEntRef leftEnt, QueryEntRef rightEnt);

	QueryEntRef getLeftEnt();
	QueryEntRef getRightEnt();

	bool isTrivialCase();
	bool executeTrivial(PKB& pkb);
	QueryResult executeNonTrivial(PKB& pkb);

private:
	QueryEntRef leftEnt;
	QueryEntRef rightEnt;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_

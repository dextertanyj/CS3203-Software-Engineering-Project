#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class ModifiesP : public Relation {
public:
	ModifiesP(QueryEntRef leftEnt, QueryEntRef rightEnt);

	QueryEntRef getLeftEnt();
	QueryEntRef getRightEnt();

	bool execute(PKB& pkb, QueryResult& result);
	bool isTrivialCase();

private:
	QueryEntRef leftEnt;
	QueryEntRef rightEnt;

	bool executeTrivial(PKB& pkb);
	bool executeNonTrivial(PKB& pkb, QueryResult& result);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H_

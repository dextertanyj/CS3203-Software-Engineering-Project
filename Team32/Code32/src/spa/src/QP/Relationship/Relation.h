#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

#include "QP/QueryResult.h"
#include "PKB/PKB.h"

class Relation {
public:
	/*
	* A relation is considered trivial if we only need to check its truth value.
	* Any relation without synonym is considered a trivial case.
	*/
	virtual bool isTrivialCase() = 0;

	virtual bool executeTrivial(PKB& pkb) = 0;
	virtual QueryResult executeNonTrivial(PKB& pkb) = 0;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

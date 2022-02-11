#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

#include "QP/QueryResult.h"
#include "QP/QueryTypeDefs.h"
#include "PKB/PKB.h"

class Relation {
public:
	/*
	* A relation is considered trivial if we only need to check its truth value.
	* Any relation without synonym is considered a trivial case.
	*/
	virtual bool isTrivialCase() = 0;
	virtual QueryResult execute(PKB& pkb, QueryResult& result) = 0;
	virtual vector<string> getDeclarationSymbols() = 0;

private:
	virtual	QueryResult executeTrivial(PKB& pkb) = 0;
	virtual QueryResult executeNonTrivial(PKB& pkb, QueryResult& result) = 0;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

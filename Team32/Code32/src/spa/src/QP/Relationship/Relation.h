#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

#include "QP/QueryResult.h"
#include "QP/QueryTypeDefs.h"
#include "QP/QueryUtils.h"
#include "PKB/PKB.h"

class Relation {
public:
	/*
	* A trivial relation is one that does not contain synonym or contains
	* synonyms that only appear in one clause.
	*/
	virtual QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) = 0;
	virtual vector<string> getDeclarationSymbols() = 0;

private:
	virtual	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) = 0;
	virtual QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) = 0;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

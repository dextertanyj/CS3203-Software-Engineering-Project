#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

#include "PKB/Storage.h"
#include "QP/QueryResult.h"
#include "QP/Relationship/Relationship.h"
#include "QP/QueryTypes.h"
#include "QP/QueryUtils.h"

class QP::Relationship::Relation {
public:
	/*
	 * A trivial relation is one that does not contain synonym or contains
	 * synonyms that only appear in one clause.
	 */
	virtual QueryResult execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) = 0;
	virtual vector<string> getDeclarationSymbols() = 0;
	virtual ~Relation() = default;

private:
	virtual QueryResult executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) = 0;
	virtual QueryResult executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) = 0;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

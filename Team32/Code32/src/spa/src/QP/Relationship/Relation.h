#ifndef SPA_SRC_QP_RELATIONSHIP_RELATION_H
#define SPA_SRC_QP_RELATIONSHIP_RELATION_H

#include "PKB/Storage.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/QueryUtils.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Relation {
public:
	/*
	 * A trivial relation is one that does not contain synonym or contains
	 * synonyms that only appear in one clause.
	 */
	inline QueryResult execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
		return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
	};
	virtual vector<string> getDeclarationSymbols() = 0;
	virtual ~Relation() = default;

private:
	virtual QueryResult executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) = 0;
	virtual QueryResult executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) = 0;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATION_H

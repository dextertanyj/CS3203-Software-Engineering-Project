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
	inline QueryResult execute(PKB::StorageAccessInterface& pkb, bool is_trivial) {
		return is_trivial ? executeTrivial(pkb) : executeNonTrivial(pkb);
	};
	virtual vector<string> getDeclarationSymbols() = 0;
	virtual ~Relation() = default;

private:
	virtual QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) = 0;
	virtual QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) = 0;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATION_H

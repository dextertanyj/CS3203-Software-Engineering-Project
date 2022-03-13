#ifndef SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H
#define SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H

#include "PKB/StorageAccessInterface.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::ModifiesS {
public:
	// Trivial Executors
	static QueryResult executeTrivialIndexName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
	                                           const Types::ReferenceArgument& ent);
	static QueryResult executeTrivialIndexWildcardOrSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt);
	static QueryResult executeTrivialSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
	                                             const Types::ReferenceArgument& ent);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt);

	// Executors
	static QueryResult executeSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
	                                      const Types::ReferenceArgument& ent);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
	                                         const Types::ReferenceArgument& ent);
	static QueryResult executeIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
	                                       const Types::ReferenceArgument& ent);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H

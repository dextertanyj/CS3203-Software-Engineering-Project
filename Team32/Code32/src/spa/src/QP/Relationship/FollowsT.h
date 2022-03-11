#ifndef SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H
#define SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H

#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "PKB/StorageAccessInterface.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::FollowsT {
public:
	// Trivial Executors

	static QueryResult executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
	                                            const Types::ReferenceArgument& rear);
	static QueryResult executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front);
	static QueryResult executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
	                                              const Types::ReferenceArgument& rear);
	static QueryResult executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& rear);
	static QueryResult executeTrivialWildcardWildcard(PKB::StorageAccessInterface& pkb);
	static QueryResult executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& rear);
	static QueryResult executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
	                                              const Types::ReferenceArgument& rear);
	static QueryResult executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front);
	static QueryResult executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
	                                                const Types::ReferenceArgument& rear);

	// Executors

	static QueryResult executeIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front, const Types::ReferenceArgument& rear);
	static QueryResult executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& rear);
	static QueryResult executeSynonymIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front, const Types::ReferenceArgument& rear);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
	                                         const Types::ReferenceArgument& rear);

	static Types::ArgumentDispatcher dispatcher;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H

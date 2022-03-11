#ifndef SPA_SRC_QP_RELATIONSHIP_CALLS_H
#define SPA_SRC_QP_RELATIONSHIP_CALLS_H

#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "PKB/StorageAccessInterface.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Calls {
public:
	// Trivial Executors
	static QueryResult executeTrivialNameName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
	                                          const Types::ReferenceArgument& callee);
	static QueryResult executeTrivialNameWildcardOrSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller);
	static QueryResult executeTrivialWildcardOrSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& callee);
	static QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(PKB::StorageAccessInterface& pkb);
	static QueryResult executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
	                                                const Types::ReferenceArgument& callee);

	// Executors
	static QueryResult executeNameSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
	                                      const Types::ReferenceArgument& callee);
	static QueryResult executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& callee);
	static QueryResult executeSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
	                                      const Types::ReferenceArgument& callee);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
	                                         const Types::ReferenceArgument& callee);

	static Types::ArgumentDispatcher dispatcher;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLS_H

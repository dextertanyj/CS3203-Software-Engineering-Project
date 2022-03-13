#ifndef SPA_SRC_QP_RELATIONSHIP_PARENT_H
#define SPA_SRC_QP_RELATIONSHIP_PARENT_H

#include "PKB/StorageAccessInterface.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Parent {
public:
	// Trivial Executors
	static QueryResult executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
	                                            const Types::ReferenceArgument& child);
	static QueryResult executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent);
	static QueryResult executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
	                                              const Types::ReferenceArgument& child);
	static QueryResult executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& child);
	static QueryResult executeTrivialWildcardWildcard(PKB::StorageAccessInterface& pkb);
	static QueryResult executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& child);
	static QueryResult executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
	                                              const Types::ReferenceArgument& child);
	static QueryResult executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent);
	static QueryResult executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
	                                                const Types::ReferenceArgument& child);

	// Executors
	static QueryResult executeIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
	                                       const Types::ReferenceArgument& child);
	static QueryResult executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& child);
	static QueryResult executeSynonymIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
	                                       const Types::ReferenceArgument& child);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
	                                         const Types::ReferenceArgument& child);

	static Types::ArgumentDispatcher dispatcher;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PARENT_H

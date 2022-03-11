#ifndef SPA_SRC_QP_RELATIONSHIP_USESP_H
#define SPA_SRC_QP_RELATIONSHIP_USESP_H

#include "PKB/StorageAccessInterface.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::UsesP {
public:
	// Trivial Executors

	static QueryResult executeTrivialNameName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& left_ent,
	                                          const Types::ReferenceArgument& right_ent);
	static QueryResult executeTrivialNameWildcardOrSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& left_ent);
	static QueryResult executeTrivialSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& right_ent);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(PKB::StorageAccessInterface& pkb);

	// Executors

	static QueryResult executeNameSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& left_ent,
	                                      const Types::ReferenceArgument& right_ent);
	static QueryResult executeSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& left_ent,
	                                      const Types::ReferenceArgument& right_ent);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& left_ent);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& left_ent,
	                                         const Types::ReferenceArgument& right_ent);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_USESP_H

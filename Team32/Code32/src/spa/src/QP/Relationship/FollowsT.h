#ifndef SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H
#define SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Follows.h"

class QP::Relationship::FollowsT : public Follows {
public:
	using Follows::Follows;

	// Trivial Executors

	static QueryResult executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
	                                            const ReferenceArgument& rear);
	static QueryResult executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front);
	static QueryResult executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
	                                              const ReferenceArgument& rear);
	static QueryResult executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& rear);
	static QueryResult executeTrivialWildcardWildcard(PKB::StorageAccessInterface& pkb);
	static QueryResult executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& rear);
	static QueryResult executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
	                                              const ReferenceArgument& rear);
	static QueryResult executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front);
	static QueryResult executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
	                                                const ReferenceArgument& rear);

	// Executors

	static QueryResult executeIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
	                                       const ReferenceArgument& rear);
	static QueryResult executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& rear);
	static QueryResult executeSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
	                                       const ReferenceArgument& rear);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
	                                         const ReferenceArgument& rear);

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H

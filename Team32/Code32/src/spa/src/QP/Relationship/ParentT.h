#ifndef SPA_SRC_QP_RELATIONSHIP_PARENTT_H
#define SPA_SRC_QP_RELATIONSHIP_PARENTT_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Parent.h"

class QP::Relationship::ParentT : public Parent {
public:
	using Parent::Parent;

	// Trivial Executors

	static QueryResult executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
	                                            const ReferenceArgument& child);
	static QueryResult executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent);
	static QueryResult executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
	                                              const ReferenceArgument& child);
	static QueryResult executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child);
	static QueryResult executeTrivialWildcardWildcard(PKB::StorageAccessInterface& pkb);
	static QueryResult executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child);
	static QueryResult executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
	                                              const ReferenceArgument& child);
	static QueryResult executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent);
	static QueryResult executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
	                                                const ReferenceArgument& child);

	// Executors

	static QueryResult executeIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
	                                       const ReferenceArgument& child);
	static QueryResult executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child);
	static QueryResult executeSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
	                                       const ReferenceArgument& child);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
	                                         const ReferenceArgument& child);

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PARENTT_H

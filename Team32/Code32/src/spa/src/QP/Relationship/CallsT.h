#ifndef SPA_SRC_QP_RELATIONSHIP_CALLST_H
#define SPA_SRC_QP_RELATIONSHIP_CALLST_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Calls.h"

class QP::Relationship::CallsT : public Calls {
public:
	using Calls::Calls;

	// Trivial Executors

	static QueryResult executeTrivialNameName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
	                                          const ReferenceArgument& callee);
	static QueryResult executeTrivialNameWildcardOrSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller);
	static QueryResult executeTrivialWildcardOrSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& callee);
	static QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(PKB::StorageAccessInterface& pkb);
	static QueryResult executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
	                                                const ReferenceArgument& callee);

	// Executors

	static QueryResult executeNameSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
	                                      const ReferenceArgument& callee);
	static QueryResult executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& callee);
	static QueryResult executeSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
	                                      const ReferenceArgument& callee);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
	                                         const ReferenceArgument& callee);

	static ArgumentDispatcher dispatcher;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLT_H

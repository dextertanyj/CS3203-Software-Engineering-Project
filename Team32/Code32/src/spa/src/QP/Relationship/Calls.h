#ifndef SPA_SRC_QP_RELATIONSHIP_CALLS_H
#define SPA_SRC_QP_RELATIONSHIP_CALLS_H

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Calls : public Relation {
public:
	Calls(ReferenceArgument caller_ent, ReferenceArgument callee_ent);

	ReferenceArgument getCallerEnt();
	ReferenceArgument getCalleeEnt();

	vector<string> getDeclarationSymbols() override;

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

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
	ReferenceArgument caller_ent;
	ReferenceArgument callee_ent;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLS_H

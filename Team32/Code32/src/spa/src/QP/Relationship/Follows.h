#ifndef SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H
#define SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Follows : public Relation {
public:
	Follows(ReferenceArgument left_stmt, ReferenceArgument right_stmt);

	ReferenceArgument getLeftStmt();
	ReferenceArgument getRightStmt();

	vector<string> getDeclarationSymbols() override;

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

	static QueryResult executeIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front, const ReferenceArgument& rear);
	static QueryResult executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& rear);
	static QueryResult executeSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front, const ReferenceArgument& rear);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
	                                         const ReferenceArgument& rear);

private:
	ReferenceArgument left_stmt;
	ReferenceArgument right_stmt;
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H

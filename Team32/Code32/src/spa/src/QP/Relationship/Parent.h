#ifndef SPA_SRC_QP_RELATIONSHIP_PARENT_H
#define SPA_SRC_QP_RELATIONSHIP_PARENT_H

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::Parent : public Relation {
public:
	Parent(ReferenceArgument parent_stmt, ReferenceArgument child_stmt);

	ReferenceArgument getParentStmt();
	ReferenceArgument getChildStmt();

	vector<string> getDeclarationSymbols() override;

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
	ReferenceArgument parent_stmt;
	ReferenceArgument child_stmt;
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PARENT_H

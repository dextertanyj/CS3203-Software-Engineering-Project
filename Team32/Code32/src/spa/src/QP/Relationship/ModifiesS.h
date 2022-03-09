#ifndef SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H
#define SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::ModifiesS : public Relation {
public:
	ModifiesS(ReferenceArgument stmt, ReferenceArgument ent);

	ReferenceArgument getStmt();
	ReferenceArgument getEnt();

	vector<string> getDeclarationSymbols() override;

	// Trivial Executors

	static QueryResult executeTrivialIndexName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt,
	                                           const ReferenceArgument& ent);
	static QueryResult executeTrivialIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt);
	static QueryResult executeTrivialSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt,
	                                             const ReferenceArgument& ent);
	static QueryResult executeTrivialSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt);

	// Executors

	static QueryResult executeSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt, const ReferenceArgument& ent);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt, const ReferenceArgument& ent);
	static QueryResult executeIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt, const ReferenceArgument& ent);

private:
	ReferenceArgument stmt;
	ReferenceArgument ent;

	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H

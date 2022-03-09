#ifndef SPA_SRC_QP_RELATIONSHIP_USESP_H
#define SPA_SRC_QP_RELATIONSHIP_USESP_H

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::UsesP : public Relation {
public:
	UsesP(ReferenceArgument left_ent, ReferenceArgument right_ent);

	ReferenceArgument getLeftEnt();
	ReferenceArgument getRightEnt();

	vector<string> getDeclarationSymbols() override;

	// Trivial Executors

	static QueryResult executeTrivialNameName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& left_ent,
	                                          const ReferenceArgument& right_ent);
	static QueryResult executeTrivialName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& left_ent);
	static QueryResult executeTrivialSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& right_ent);
	static QueryResult executeTrivialSynonym(PKB::StorageAccessInterface& pkb);

	// Executors

	static QueryResult executeNameSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& left_ent,
	                                      const ReferenceArgument& right_ent);
	static QueryResult executeSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& left_ent,
	                                      const ReferenceArgument& right_ent);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& left_ent);
	static QueryResult executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& left_ent,
	                                         const ReferenceArgument& right_ent);

private:
	ReferenceArgument left_ent;
	ReferenceArgument right_ent;
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_USESP_H

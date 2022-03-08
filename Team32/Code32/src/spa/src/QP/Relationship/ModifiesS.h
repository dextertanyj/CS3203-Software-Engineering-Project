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

private:
	ReferenceArgument stmt;
	ReferenceArgument ent;

	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;

	QueryResult executeTrivialEntVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeTrivialEntUnknown(PKB::StorageAccessInterface& pkb);

	QueryResult executeNonTrivialEntVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialEntUnderscore(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialEntSynonym(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialStmtStmtNo(PKB::StorageAccessInterface& pkb);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H

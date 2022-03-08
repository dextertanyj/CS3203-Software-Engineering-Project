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

private:
	ReferenceArgument left_ent;
	ReferenceArgument right_ent;

	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;

	QueryResult executeTrivialBothUnknown(PKB::StorageAccessInterface& pkb);

	QueryResult executeNonTrivialLeftEntVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialRightEntVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialRightEntUnderscore(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialRightEntSynonym(PKB::StorageAccessInterface& pkb);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_USESP_H

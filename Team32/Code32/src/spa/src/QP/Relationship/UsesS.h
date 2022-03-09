#ifndef SPA_SRC_QP_RELATIONSHIP_USESS_H
#define SPA_SRC_QP_RELATIONSHIP_USESS_H

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::UsesS : public Relation {
public:
	UsesS(ReferenceArgument stmt, ReferenceArgument ent);

	ReferenceArgument getStmt();
	ReferenceArgument getEnt();

	vector<string> getDeclarationSymbols() override;

private:
	ReferenceArgument stmt;
	ReferenceArgument ent;

	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;

	QueryResult executeTrivialEntVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeTrivialEntUnknown(PKB::StorageAccessInterface& pkb);

	QueryResult executeNonTrivialEntVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialEntUnderscore(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialEntSynonym(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialStmtStmtNo(PKB::StorageAccessInterface& pkb);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_USESS_H

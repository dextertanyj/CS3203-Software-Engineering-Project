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

protected:
	ReferenceArgument caller_ent;
	ReferenceArgument callee_ent;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;

	QueryResult executeTrivialCallerVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeTrivialCallerUnderscoreSynonym(PKB::StorageAccessInterface& pkb);

	QueryResult executeNonTrivialCallerVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialCallerUnderscore(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialCallerSynonym(PKB::StorageAccessInterface& pkb);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLS_H

#ifndef SPA_SRC_QP_RELATIONSHIP_CALLS_H
#define SPA_SRC_QP_RELATIONSHIP_CALLS_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Relation.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Calls : public Relation {
public:
	Calls(QueryEntRef caller_ent, QueryEntRef callee_ent);

	QueryEntRef getCallerEnt();
	QueryEntRef getCalleeEnt();

	vector<string> getDeclarationSymbols() override;

protected:
	QueryEntRef caller_ent;
	QueryEntRef callee_ent;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;

	QueryResult executeTrivialCallerVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeTrivialCallerUnderscoreSynonym(PKB::StorageAccessInterface& pkb);

	QueryResult executeNonTrivialCallerVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialCallerUnderscore(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialCallerSynonym(PKB::StorageAccessInterface& pkb);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLS_H

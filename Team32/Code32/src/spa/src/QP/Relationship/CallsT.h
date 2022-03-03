#ifndef SPA_SRC_QP_RELATIONSHIP_CALLST_H
#define SPA_SRC_QP_RELATIONSHIP_CALLST_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Calls.h"

class QP::Relationship::CallsT : public Calls {
public:
	using Calls::Calls;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;

	QueryResult executeTrivialCallerVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeTrivialCallerUnderscoreSynonym(PKB::StorageAccessInterface& pkb);

	QueryResult executeNonTrivialCallerVarName(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialCallerUnderscore(PKB::StorageAccessInterface& pkb);
	QueryResult executeNonTrivialCallerSynonym(PKB::StorageAccessInterface& pkb);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLT_H

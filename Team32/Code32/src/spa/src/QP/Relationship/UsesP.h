#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_

#include "QP/QueryTypeDefs.h"
#include "QP/Relationship/Relation.h"

class UsesP : public Relation {
public:
	UsesP(QueryEntRef left_ent, QueryEntRef right_ent);

	QueryEntRef getLeftEnt();
	QueryEntRef getRightEnt();

	QueryResult execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;
	vector<string> getDeclarationSymbols() override;

private:
	QueryEntRef left_ent;
	QueryEntRef right_ent;

	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESP_H_

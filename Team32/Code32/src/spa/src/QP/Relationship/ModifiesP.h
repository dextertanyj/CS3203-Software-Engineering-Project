#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H_

#include "QP/QueryTypes.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::ModifiesP : public Relation {
public:
	ModifiesP(QueryEntRef left_ent, QueryEntRef right_ent);

	QueryEntRef getLeftEnt();
	QueryEntRef getRightEnt();

	QueryResult execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;
	vector<string> getDeclarationSymbols() override;

private:
	QueryEntRef left_ent;
	QueryEntRef right_ent;

	QueryResult executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H_

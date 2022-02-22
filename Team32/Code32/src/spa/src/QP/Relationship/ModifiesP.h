#ifndef SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H
#define SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H

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

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIESP_H

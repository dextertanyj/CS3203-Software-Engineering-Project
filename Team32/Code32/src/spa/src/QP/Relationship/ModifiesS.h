#ifndef SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H
#define SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::ModifiesS : public Relation {
public:
	ModifiesS(QueryStmtRef stmt, QueryEntRef ent);

	QueryStmtRef getStmt();
	QueryEntRef getEnt();

	vector<string> getDeclarationSymbols() override;

private:
	QueryStmtRef stmt;
	QueryEntRef ent;

	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H

#ifndef SPA_SRC_QP_RELATIONSHIP_USESS_H
#define SPA_SRC_QP_RELATIONSHIP_USESS_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::UsesS : public Relation {
public:
	UsesS(QueryStmtRef stmt, QueryEntRef ent);

	QueryStmtRef getStmt();
	QueryEntRef getEnt();

	vector<string> getDeclarationSymbols() override;

private:
	QueryStmtRef stmt;
	QueryEntRef ent;

	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;

	QueryResult executeTrivialEntVarName(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeTrivialEntUnknown(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map);

	QueryResult executeNonTrivialEntVarName(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivialEntUnderscore(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivialEntSynonym(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivialStmtStmtNo(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_USESS_H

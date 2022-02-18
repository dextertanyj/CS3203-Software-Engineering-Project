#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_

#include "QP/QueryTypeDefs.h"
#include "QP/Relationship/Relation.h"

class UsesS : public Relation {
public:
	UsesS(QueryStmtRef stmt, QueryEntRef ent);

	QueryStmtRef getStmt();
	QueryEntRef getEnt();

	QueryResult execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;
	vector<string> getDeclarationSymbols() override;

private:
	QueryStmtRef stmt;
	QueryEntRef ent;

	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_

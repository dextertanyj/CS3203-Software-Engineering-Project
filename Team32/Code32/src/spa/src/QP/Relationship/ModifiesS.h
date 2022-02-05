#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H_

#include "Relation.h"
#include "QP/QueryTypeDefs.h"

class ModifiesS : public Relation {
public:
	ModifiesS(QueryStmtRef stmt, QueryEntRef ent);

	QueryStmtRef getStmt();
	QueryEntRef getEnt();

	bool isTrivialCase();
	bool execute(PKB& pkb, QueryResult& result);

private:
	QueryStmtRef stmt;
	QueryEntRef ent;

	bool executeTrivial(PKB& pkb);
	bool executeNonTrivial(PKB& pkb, QueryResult& result);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H_

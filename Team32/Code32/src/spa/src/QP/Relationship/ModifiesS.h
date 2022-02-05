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
	bool executeTrivial(PKB& pkb);
	QueryResult executeNonTrivial(PKB& pkb);

private:
	QueryStmtRef stmt;
	QueryEntRef ent;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_MODIFIESS_H_

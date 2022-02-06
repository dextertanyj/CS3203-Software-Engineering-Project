#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class UsesS : public Relation {
public:
	UsesS(QueryStmtRef stmt, QueryEntRef ent);

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

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_

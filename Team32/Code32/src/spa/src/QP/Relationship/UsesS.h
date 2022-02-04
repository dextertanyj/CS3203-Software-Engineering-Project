#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_

#include "Relation.h"
#include "QP/QueryTypeDefs.h"

class UsesS : public Relation {
public:
	UsesS(QueryStmtRef stmt, QueryEntRef ent);

	QueryStmtRef getStmt();
	QueryEntRef getEnt();

	QueryResult execute();

private:
	QueryStmtRef stmt;
	QueryEntRef ent;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_USESS_H_

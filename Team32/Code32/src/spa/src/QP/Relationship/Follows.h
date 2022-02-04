#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

#include "Relation.h"
#include "QP/QueryTypeDefs.h"

class Follows : public Relation {
public:
	Follows(bool isStar, QueryStmtRef leftStmt, QueryStmtRef rightStmt);

	QueryStmtRef getLeftStmt();
	QueryStmtRef getRightStmt();
	bool getIsStar();

	QueryResult execute();

private:
	bool isStar;
	QueryStmtRef leftStmt;
	QueryStmtRef rightStmt;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class Follows : public Relation {
public:
	Follows(bool isStar, QueryStmtRef leftStmt, QueryStmtRef rightStmt);

	QueryStmtRef getLeftStmt();
	QueryStmtRef getRightStmt();
	bool getIsStar();

	bool isTrivialCase();
	bool execute(PKB& pkb, QueryResult& result);

private:
	bool isStar;
	QueryStmtRef leftStmt;
	QueryStmtRef rightStmt;

	bool executeTrivial(PKB& pkb);
	bool executeNonTrivial(PKB& pkb, QueryResult& result);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

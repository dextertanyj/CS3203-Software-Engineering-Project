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

	QueryResult execute(PKB& pkb, bool isTrivial);
	vector<string> getDeclarationSymbols();

private:
	bool isStar;
	QueryStmtRef leftStmt;
	QueryStmtRef rightStmt;

	QueryResult executeTrivial(PKB& pkb);
	QueryResult executeNonTrivial(PKB& pkb);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

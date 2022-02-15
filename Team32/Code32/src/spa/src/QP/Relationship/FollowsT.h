#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class FollowsT : public Relation {
public:
	FollowsT( QueryStmtRef leftStmt, QueryStmtRef rightStmt);

	QueryStmtRef getLeftStmt();
	QueryStmtRef getRightStmt();

	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);
	vector<string> getDeclarationSymbols();

private:
	QueryStmtRef leftStmt;
	QueryStmtRef rightStmt;

	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_

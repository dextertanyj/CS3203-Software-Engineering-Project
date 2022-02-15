#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class Follows : public Relation {
public:
	Follows(QueryStmtRef leftStmt, QueryStmtRef rightStmt);

	QueryStmtRef getLeftStmt();
	QueryStmtRef getRightStmt();
	bool getIsStar();

	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);
	vector<string> getDeclarationSymbols();

protected:
	QueryStmtRef leftStmt;
	QueryStmtRef rightStmt;

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

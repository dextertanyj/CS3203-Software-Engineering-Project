#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class Parent : public Relation {
public:
	Parent(bool isStar, QueryStmtRef parentStmt, QueryStmtRef childStmt);

	QueryStmtRef getParentStmt();
	QueryStmtRef getChildStmt();
	bool getIsStar();

	bool isTrivialCase();
	QueryResult execute(PKB& pkb, QueryResult& result);
	vector<string> getDeclarationSymbols();

private:
	bool isStar;
	QueryStmtRef parentStmt;
	QueryStmtRef childStmt;

	QueryResult executeTrivial(PKB& pkb);
	QueryResult executeNonTrivial(PKB& pkb, QueryResult& result);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_

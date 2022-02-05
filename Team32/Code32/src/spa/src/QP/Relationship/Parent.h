#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_

#include "Relation.h"
#include "QP/QueryTypeDefs.h"

class Parent : public Relation {
public:
	Parent(bool isStar, QueryStmtRef parentStmt, QueryStmtRef childStmt);

	QueryStmtRef getParentStmt();
	QueryStmtRef getChildStmt();
	bool getIsStar();

	bool isTrivialCase();
	bool executeTrivial(PKB& pkb);
	QueryResult executeNonTrivial(PKB& pkb);

private:
	bool isStar;
	QueryStmtRef parentStmt;
	QueryStmtRef childStmt;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_

#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class Parent : public Relation {
public:
	Parent(QueryStmtRef parentStmt, QueryStmtRef childStmt);

	QueryStmtRef getParentStmt();
	QueryStmtRef getChildStmt();
	bool getIsStar();

	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);
	vector<string> getDeclarationSymbols();

protected:
	QueryStmtRef parentStmt;
	QueryStmtRef childStmt;

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENT_H_

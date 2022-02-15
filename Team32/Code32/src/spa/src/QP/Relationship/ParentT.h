#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_

#include "QP/Relationship/Relation.h"
#include "QP/QueryTypeDefs.h"

class ParentT : public Relation {
public:
	ParentT(QueryStmtRef parentStmt, QueryStmtRef childStmt);

	QueryStmtRef getParentStmt();
	QueryStmtRef getChildStmt();

	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);
	vector<string> getDeclarationSymbols();

private:
	QueryStmtRef parentStmt;
	QueryStmtRef childStmt;

	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_

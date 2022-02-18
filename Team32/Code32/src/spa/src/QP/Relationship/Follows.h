#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

#include "QP/QueryTypeDefs.h"
#include "QP/Relationship/Relation.h"

class Follows : public Relation {
public:
	Follows(QueryStmtRef left_stmt, QueryStmtRef right_stmt);

	QueryStmtRef getLeftStmt();
	QueryStmtRef getRightStmt();

	QueryResult execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;
	vector<string> getDeclarationSymbols() override;

private:
	QueryStmtRef left_stmt;
	QueryStmtRef right_stmt;
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H_

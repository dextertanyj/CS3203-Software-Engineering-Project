#ifndef SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H
#define SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Relation.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Follows : public Relation {
public:
	Follows(QueryStmtRef left_stmt, QueryStmtRef right_stmt);

	QueryStmtRef getLeftStmt();
	QueryStmtRef getRightStmt();

	vector<string> getDeclarationSymbols() override;

private:
	QueryStmtRef left_stmt;
	QueryStmtRef right_stmt;
	QueryResult executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H

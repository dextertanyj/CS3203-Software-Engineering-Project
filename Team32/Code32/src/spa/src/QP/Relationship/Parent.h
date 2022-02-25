#ifndef SPA_SRC_QP_RELATIONSHIP_PARENT_H
#define SPA_SRC_QP_RELATIONSHIP_PARENT_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::Parent : public Relation {
public:
	Parent(QueryStmtRef parent_stmt, QueryStmtRef child_stmt);

	QueryStmtRef getParentStmt();
	QueryStmtRef getChildStmt();

	QueryResult execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;
	vector<string> getDeclarationSymbols() override;

private:
	QueryStmtRef parent_stmt;
	QueryStmtRef child_stmt;
	QueryResult executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PARENT_H

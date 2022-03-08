#ifndef SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H
#define SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Follows : public Relation {
public:
	Follows(ReferenceArgument left_stmt, ReferenceArgument right_stmt);

	ReferenceArgument getLeftStmt();
	ReferenceArgument getRightStmt();

	vector<string> getDeclarationSymbols() override;

private:
	ReferenceArgument left_stmt;
	ReferenceArgument right_stmt;
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_FOLLOWS_H

#ifndef SPA_SRC_QP_RELATIONSHIP_PARENT_H
#define SPA_SRC_QP_RELATIONSHIP_PARENT_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Relation.h"
#include "QP/ReferenceArgument.h"

class QP::Relationship::Parent : public Relation {
public:
	Parent(ReferenceArgument parent_stmt, ReferenceArgument child_stmt);

	ReferenceArgument getParentStmt();
	ReferenceArgument getChildStmt();

	vector<string> getDeclarationSymbols() override;

private:
	ReferenceArgument parent_stmt;
	ReferenceArgument child_stmt;
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PARENT_H

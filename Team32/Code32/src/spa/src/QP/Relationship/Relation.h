#ifndef SPA_SRC_QP_RELATIONSHIP_RELATION_H
#define SPA_SRC_QP_RELATIONSHIP_RELATION_H

#include "PKB/StorageAccessInterface.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Relation {
public:
	Relation(Types::ClauseType type, vector<Types::ReferenceArgument> arguments, Types::ExecutorSet executor);
	[[nodiscard]] QueryResult execute(PKB::StorageAccessInterface& pkb, bool is_trivial) const;
	[[nodiscard]] vector<string> getDeclarationSymbols() const;
	[[nodiscard]] Types::ClauseType getType() const;

private:
	Types::ClauseType type;
	vector<Types::ReferenceArgument> arguments;
	Types::ExecutorSet executor;
	[[nodiscard]] QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) const;
	[[nodiscard]] QueryResult execute(PKB::StorageAccessInterface& pkb) const;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATION_H

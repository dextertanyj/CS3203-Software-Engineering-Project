#ifndef SPA_SRC_QP_RELATIONSHIP_RELATION_H
#define SPA_SRC_QP_RELATIONSHIP_RELATION_H

#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

class QP::Relationship::Relation {
public:
	Relation(Types::ClauseType type, vector<Types::ReferenceArgument> arguments, Types::ExecutorSet executor);
	[[nodiscard]] QueryResult executeTrivial(const StorageAdapter& pkb) const;
	[[nodiscard]] QueryResult execute(const StorageAdapter& pkb, vector<QueryResult>& results) const;
	[[nodiscard]] vector<string> getDeclarationSymbols() const;
	[[nodiscard]] vector<Types::ReferenceArgument> getArguments() const;
	[[nodiscard]] Types::ClauseType getType() const;
	[[nodiscard]] size_t getCost() const;
	[[nodiscard]] bool operator==(const Relation& other) const;

private:
	Types::ClauseType type;
	vector<Types::ReferenceArgument> arguments;
	Types::ExecutorSet executor;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATION_H

#ifndef SPA_SRC_QP_RELATIONSHIP_RELATION_H
#define SPA_SRC_QP_RELATIONSHIP_RELATION_H

#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"
#include "QP/StorageAdapter.h"

class QP::Relationship::Relation {
public:
	Relation(Types::ClauseType type, vector<Types::ReferenceArgument> arguments, Types::ExecutorSet executor);
	[[nodiscard]] QueryResult execute(QP::StorageAdapter& pkb, bool is_trivial) const;
	[[nodiscard]] vector<string> getDeclarationSymbols() const;
	[[nodiscard]] Types::ClauseType getType() const;

private:
	Types::ClauseType type;
	vector<Types::ReferenceArgument> arguments;
	Types::ExecutorSet executor;
	[[nodiscard]] QueryResult executeTrivial(QP::StorageAdapter& pkb) const;
	[[nodiscard]] QueryResult execute(QP::StorageAdapter& pkb) const;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATION_H

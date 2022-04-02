#ifndef SPA_SRC_QP_RELATIONSHIP_RELATION_H
#define SPA_SRC_QP_RELATIONSHIP_RELATION_H

#include "QP/Evaluator/Evaluator.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

class QP::Evaluator::Clause {
public:
	Clause(Types::ClauseType type, vector<ReferenceArgument> arguments, Types::ExecutorSet executor);
	[[nodiscard]] QueryResult executeTrivial(const StorageAdapter& pkb) const;
	[[nodiscard]] QueryResult execute(const StorageAdapter& pkb, vector<QueryResult>& results) const;
	[[nodiscard]] vector<string> getDeclarationSymbols() const;
	[[nodiscard]] Types::ClauseType getType() const;
	[[nodiscard]] size_t getCost() const;

private:
	Types::ClauseType type;
	vector<ReferenceArgument> arguments;
	Types::ExecutorSet executor;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATION_H

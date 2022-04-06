#ifndef SPA_SRC_QP_RELATIONSHIP_RELATION_H
#define SPA_SRC_QP_RELATIONSHIP_RELATION_H

#include "QP/Evaluator/Evaluator.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

class QP::Evaluator::Clause {
public:
	Clause(Types::ClauseType type, std::vector<ReferenceArgument> arguments, Types::ExecutorSet executor);
	[[nodiscard]] QueryResult executeTrivial(const StorageAdapter& pkb) const;
	[[nodiscard]] QueryResult execute(const StorageAdapter& pkb, std::vector<QueryResult>& results) const;
	[[nodiscard]] std::vector<std::string> getDeclarationSymbols() const;
	[[nodiscard]] Types::ClauseType getType() const;
	[[nodiscard]] size_t getCost() const;
	[[nodiscard]] bool operator==(const Clause& other) const;

	friend std::hash<QP::Evaluator::Clause>;

private:
	Types::ClauseType type;
	vector<ReferenceArgument> arguments;
	Types::ExecutorSet executor;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATION_H
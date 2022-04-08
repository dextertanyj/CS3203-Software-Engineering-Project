#ifndef SPA_SRC_QP_QUERYEVALUATOR_H
#define SPA_SRC_QP_QUERYEVALUATOR_H

#include <string>
#include <vector>

#include "QP/Evaluator/Evaluator.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

class QP::Evaluator::QueryEvaluator {
public:
	explicit QueryEvaluator(QP::StorageAdapter& store);
	[[nodiscard]] QueryResult executeQuery(const QueryProperties& query_properties) const;

private:
	const QP::StorageAdapter& store;

	[[nodiscard]] bool executeGroup(const Types::ClauseList& clauses, const Types::DeclarationList& select_list,
	                                std::vector<QueryResult>& results) const;
	[[nodiscard]] QueryResult executeGroupWithSelected(const Types::ClauseList& clauses, const Types::DeclarationList& select_list) const;
	[[nodiscard]] QueryResult executeGroupWithoutSelected(const Types::ClauseList& clauses,
	                                                      const Types::DeclarationList& select_list) const;
	[[nodiscard]] QueryResult executeTrivialGroup(const Types::ClauseList& clauses) const;
	[[nodiscard]] QueryResult executeNonTrivialGroup(const Types::ClauseList& clauses, const Types::DeclarationList& select_list) const;
	[[nodiscard]] QueryResult executeNoClauses(const Types::Declaration& select) const;
	[[nodiscard]] QueryResult getSpecificStmtType(const Types::Declaration& declaration) const;
	[[nodiscard]] QueryResult getConstants(const std::string& symbol) const;
	[[nodiscard]] QueryResult getVariables(const std::string& symbol) const;
	[[nodiscard]] QueryResult getProcedures(const std::string& symbol) const;

	[[nodiscard]] static Types::ClauseList getClausesWithoutSynonyms(const QueryProperties& query_properties);
};

#endif  // SPA_SRC_QP_QUERYEVALUATOR_H

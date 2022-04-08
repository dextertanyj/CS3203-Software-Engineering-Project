#ifndef SPA_SRC_QP_QUERYEVALUATOR_H
#define SPA_SRC_QP_QUERYEVALUATOR_H

#include <string>
#include <vector>

#include "QP/Evaluator/Evaluator.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

using QP::Types::ClauseList;
using QP::Types::Declaration;
using QP::Types::DeclarationList;

class QP::Evaluator::QueryEvaluator {
public:
	explicit QueryEvaluator(QP::StorageAdapter& store);
	QueryResult executeQuery(QueryProperties& query_properties);

private:
	const QP::StorageAdapter& store;
	bool executeGroup(ClauseList& clauses, DeclarationList& select_list, std::vector<QueryResult>& results);
	QueryResult executeGroupWithSelected(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeGroupWithoutSelected(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeTrivialGroup(ClauseList& clauses);
	QueryResult executeNonTrivialGroup(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeNoClauses(const Declaration& select);
	QueryResult getSpecificStmtType(const Declaration& declaration);
	QueryResult getConstants(const std::string& symbol);
	QueryResult getVariables(const std::string& symbol);
	QueryResult getProcedures(const std::string& symbol);
	static ClauseList getClausesWithoutSynonyms(QueryProperties& query_properties);
};

#endif  // SPA_SRC_QP_QUERYEVALUATOR_H

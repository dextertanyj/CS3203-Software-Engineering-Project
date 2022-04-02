#ifndef SPA_SRC_QP_QUERYEVALUATOR_H
#define SPA_SRC_QP_QUERYEVALUATOR_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "PKB/Storage.h"
#include "QP/QP.h"
#include "QP/Optimizer/QueryGraph.h"
#include "QP/QueryProperties.h"
#include "QP/Evaluator/Evaluator.h"

using namespace std;
using QP::Types::ClauseList;
using QP::Types::Declaration;
using QP::Types::DeclarationList;

class QP::Evaluator::QueryEvaluator {
public:
	explicit QueryEvaluator(QP::StorageAdapter& store);
	QueryResult executeQuery(QueryProperties& query_properties);

private:
	const QP::StorageAdapter& store;
	bool executeGroup(ClauseList& clauses, DeclarationList& select_list, vector<QueryResult>& results);
	QueryResult executeGroupWithSelected(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeGroupWithoutSelected(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeTrivialGroup(ClauseList& clauses);
	QueryResult executeNonTrivialGroup(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeNoClauses(const Declaration& select);
	QueryResult getSpecificStmtType(const Declaration& declaration);
	QueryResult getConstants(const string& symbol);
	QueryResult getVariables(const string& symbol);
	QueryResult getProcedures(const string& symbol);
	static ClauseList getClausesWithoutSynonyms(QueryProperties& query_properties);
};


#endif  // SPA_SRC_QP_QUERYEVALUATOR_H

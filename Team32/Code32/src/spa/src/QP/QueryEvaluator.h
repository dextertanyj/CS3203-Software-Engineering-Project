#ifndef SPA_SRC_QP_QUERYEVALUATOR_H
#define SPA_SRC_QP_QUERYEVALUATOR_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "PKB/Storage.h"
#include "QP/QP.h"
#include "QP/QueryGraph.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

using namespace std;

class QP::QueryEvaluator {
public:
	explicit QueryEvaluator(PKB::StorageAccessInterface& pkb);
	QP::QueryResult executeQuery(QueryProperties& query_properties);
	static vector<ClauseList> splitClauses(QueryProperties& query_properties, unordered_map<string, size_t>& synonyms_in_group);

private:
	PKB::StorageAccessInterface& pkb;
	QueryResult executeNoClauses(const Declaration& select);
	QueryResult getSpecificStmtType(DesignEntity design_entity, const string& symbol);
	QueryResult getConstants(const string& symbol);
	QueryResult getVariables(const string& symbol);
	QueryResult getProcedures(const string& symbol);
	static QueryGraph buildGraph(QueryProperties& query_properties);
	QueryResult evaluateClauses(ClauseList& clauses, bool is_trivial);
	QueryResult executeClausesWithoutSynonym(ClauseList& clauses);
	bool executeGroup(ClauseList& clauses);
};

#endif  // SPA_SRC_QP_QUERYEVALUATOR_H

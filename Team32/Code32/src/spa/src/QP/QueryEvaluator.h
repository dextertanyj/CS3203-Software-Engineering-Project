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
	static vector<pair<ClauseList, bool>> splitClauses(QueryProperties& query_properties, ConnectedSynonyms& connected_synonyms);

private:
	PKB::StorageAccessInterface& pkb;
	QueryResult executeNoClauses(const DeclarationList& select_list);
	QueryResult getSpecificStmtType(const Declaration& declaration);
	QueryResult getConstants(const string& symbol);
	QueryResult getVariables(const string& symbol);
	QueryResult getProcedures(const string& symbol);
	static QueryGraph buildGraph(QueryProperties& query_properties);
	QueryResult evaluateClauses(ClauseList& clauses, bool is_trivial);
	QueryResult executeClausesWithoutSynonym(ClauseList& clauses);
	bool executeGroup(ClauseList& clauses);
};

#endif  // SPA_SRC_QP_QUERYEVALUATOR_H

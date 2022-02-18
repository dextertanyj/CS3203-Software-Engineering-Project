#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEVALUATOR_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEVALUATOR_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "PKB/PKB.h"
#include "QP/QueryGraph.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

using namespace std;

class QueryEvaluator {
public:
	explicit QueryEvaluator(PKB& pkb);
	QueryResult executeQuery(QueryProperties& query_properties);
	static vector<pair<SuchThatClauseList, PatternClauseList>> splitClauses(QueryProperties& query_properties,
	                                                                        vector<unordered_set<string>>& synonyms_in_group);

private:
	PKB& pkb;
	unordered_map<string, DesignEntity> symbol_to_type_map;
	QueryResult executeNoClauses(const Declaration& select);
	QueryResult getSpecificStmtType(StmtType type, const string& symbol);
	static QueryGraph buildGraph(QueryProperties& query_properties);
	QueryResult evaluateClauses(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses, const Declaration& select,
	                            bool is_trivial);
	QueryResult executeClausesWithoutSynonym(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses,
	                                         const Declaration& select);
	void createSymbolToTypeMap(const DeclarationList& declarations);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEVALUATOR_H_

#ifndef SPA_SRC_QP_QUERYEVALUATOR_H
#define SPA_SRC_QP_QUERYEVALUATOR_H

#include <string>
#include <unordered_map>
#include <unordered_set>
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
	static vector<pair<SuchThatClauseList, PatternClauseList>> splitClauses(QueryProperties& query_properties,
	                                                                        vector<unordered_set<string>>& synonyms_in_group);

private:
	PKB::StorageAccessInterface& pkb;
	unordered_map<string, DesignEntity> symbol_to_type_map;
	QueryResult executeNoClauses(const Declaration& select);
	QueryResult getSpecificStmtType(DesignEntity design_entity, const string& symbol);
	QueryResult getConstants(const string& symbol);
	QueryResult getVariables(const string& symbol);
	QueryResult getProcedures(const string& symbol);
	static QueryGraph buildGraph(QueryProperties& query_properties);
	QueryResult evaluateClauses(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses, bool is_trivial);
	QueryResult executeClausesWithoutSynonym(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses);
	bool executeGroup(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses);
	void createSymbolToTypeMap(const DeclarationList& declarations);
};

#endif  // SPA_SRC_QP_QUERYEVALUATOR_H

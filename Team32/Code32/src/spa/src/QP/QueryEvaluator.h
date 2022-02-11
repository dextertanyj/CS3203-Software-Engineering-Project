#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEVALUATOR_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEVALUATOR_H_

#include <string>
#include <unordered_map>

#include "PKB/PKB.h"
#include "QP/QueryGraph.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

using namespace std;

class QueryEvaluator {
public:
	explicit QueryEvaluator(PKB& pkb);
	QueryResult executeQuery(QueryProperties& queryProperties);

private:
	PKB& pkb;
	unordered_map<string, QueryResult> results;
	QueryResult executeNoClauses(Declaration& select);
	QueryGraph buildGraph(QueryProperties& queryProperties);
	QueryResult evaluateSuchThatClauses(SuchThatClauseList& suchThatClauseList);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEVALUATOR_H_

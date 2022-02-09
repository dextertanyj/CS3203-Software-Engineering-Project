#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "PKB/PKB.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

class QueryEvaluator {
public:
	QueryEvaluator(PKB& pkb);
	QueryResult executeQuery(QueryProperties& queryProperties);

private:
	PKB& pkb;
	QueryResult evaluateSuchThatClauses(SuchThatClauseList& suchThatClauseList);
};

#endif  // QUERY_EVALUATOR_H_

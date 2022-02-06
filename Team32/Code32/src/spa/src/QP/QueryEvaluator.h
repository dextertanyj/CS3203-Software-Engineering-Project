#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"
#include "PKB/PKB.h"

class QueryEvaluator {
public:
	static QueryResult executeQuery(QueryProperties& queryProperties);

private:
	static PKB pkb;
	static QueryResult evaluateSuchThatClauses(SuchThatClauseList& suchThatClauseList);
};

#endif // QUERY_EVALUATOR_H_

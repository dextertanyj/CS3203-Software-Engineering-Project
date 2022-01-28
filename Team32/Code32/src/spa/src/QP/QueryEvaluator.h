#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "QueryProperties.h"
#include "QueryResult.h"

class QueryEvaluator {
public:
	static QueryResult executeQuery(const QueryProperties& queryProperties);
};

#endif // QUERY_EVALUATOR_H_

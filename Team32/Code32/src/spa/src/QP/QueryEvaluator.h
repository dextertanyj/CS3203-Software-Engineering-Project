#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

class QueryProperties;
class QueryResult;

class QueryEvaluator {
public:
	static QueryResult* executeQuery(QueryProperties* queryProperties);
};

#endif // QUERY_EVALUATOR_H_

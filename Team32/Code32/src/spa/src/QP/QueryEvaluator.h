#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include <memory>

using namespace std;

class QueryProperties;
class QueryResult;

class QueryEvaluator {
public:
	static unique_ptr<QueryResult> executeQuery(QueryProperties* queryProperties);
};

#endif // QUERY_EVALUATOR_H_

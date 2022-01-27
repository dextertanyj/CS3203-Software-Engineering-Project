#include "QueryEvaluator.h"

#include <memory>

#include "QueryProperties.h"
#include "QueryResult.h"

using namespace std;

unique_ptr<QueryResult> QueryEvaluator::executeQuery(QueryProperties* queryProperties){
	return make_unique<QueryResult>();
}

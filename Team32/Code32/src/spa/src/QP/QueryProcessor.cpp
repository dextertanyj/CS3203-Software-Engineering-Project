#include "QueryProcessor.h"

#include "QueryPreprocessor.h"

string QueryProcessor::processQuery(string query) {
	this->query = query;
	QueryPreprocessor queryPrepro;
	QueryProperties queryProperties = queryPrepro.parseQuery(query);
	this->queryResult = QueryEvaluator::executeQuery(queryProperties);
	return QueryFormatter::formatResult(queryProperties, this->queryResult);
}

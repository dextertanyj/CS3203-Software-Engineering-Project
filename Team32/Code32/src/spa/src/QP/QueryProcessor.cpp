#include "QP/QueryProcessor.h"

#include "QP/QueryPreprocessor.h"

QueryProcessor::QueryProcessor(PKB& pkb) : evaluator(QueryEvaluator(pkb)) {}

vector<string> QueryProcessor::processQuery(string query) {
	QueryPreprocessor preprocessor;
	QueryProperties query_properties = preprocessor.parseQuery(std::move(query));
	QueryResult result = evaluator.executeQuery(query_properties);
	return QueryFormatter::formatResult(query_properties, result);
}

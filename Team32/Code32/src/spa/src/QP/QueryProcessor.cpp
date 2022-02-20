#include "QP/QueryProcessor.h"

#include <exception>
#include <utility>

#include "QP/QueryPreprocessor.h"

QueryProcessor::QueryProcessor(PKB& pkb) : evaluator(QueryEvaluator(pkb)) {}

vector<string> QueryProcessor::processQuery(string query) {
	try {
		QueryPreprocessor preprocessor;
		QueryProperties query_properties = preprocessor.parseQuery(std::move(query));
		QueryResult result = evaluator.executeQuery(query_properties);
		return QueryFormatter::formatResult(query_properties, result);
	} catch (const exception& e) {
		return {};
	}
}

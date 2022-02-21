#include "QP/QueryProcessor.h"

#include <exception>
#include <utility>

#include "QP/QueryPreprocessor.h"

QP::QueryProcessor::QueryProcessor(PKB::Storage& pkb) : evaluator(QueryEvaluator(pkb)) {}

vector<string> QP::QueryProcessor::processQuery(string query) {
	try {
		QueryPreprocessor preprocessor;
		QueryProperties query_properties = preprocessor.parseQuery(std::move(query));
		QueryResult result = evaluator.executeQuery(query_properties);
		return QueryFormatter::formatResult(query_properties, result);
	} catch (const exception& e) {
		return {};
	}
}

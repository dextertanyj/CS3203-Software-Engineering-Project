#include "QP/QueryProcessor.h"

#include <exception>
#include <utility>

#include "QP/QueryPreprocessor.h"

QP::QueryProcessor::QueryProcessor(PKB::StorageAccessInterface& pkb) : store(pkb), evaluator(store), post_processor(store) {}

vector<string> QP::QueryProcessor::processQuery(string query) {
	try {
		QueryProperties query_properties = pre_processor.parseQuery(std::move(query));
		QueryResult result = evaluator.executeQuery(query_properties);
		return post_processor.processResult(query_properties, result);
	} catch (const exception& e) {
		return {};
	}
}

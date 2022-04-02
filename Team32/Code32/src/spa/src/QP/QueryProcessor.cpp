#include "QP/QueryProcessor.h"

#include <exception>
#include <utility>

#include "QP/Preprocessor/QueryPreprocessor.h"

QP::QueryProcessor::QueryProcessor(PKB::StorageAccessInterface& pkb) : store(pkb), evaluator(store), post_processor(store) {}

vector<string> QP::QueryProcessor::processQuery(string query) {
	try {
		// Invalidate the cache at start of each query to prevent exceptions from leaving the cache in an inconsistent state.
		store.resetCache();
		QueryProperties query_properties = pre_processor.parseQuery(std::move(query));
		QueryResult result = evaluator.executeQuery(query_properties);
		return post_processor.processResult(query_properties, result);
	} catch (const QP::QuerySemanticException& e) {
		return e.result;
	} catch (const exception& e) {
		return {};
	}
}

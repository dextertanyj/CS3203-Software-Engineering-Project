#include "QP/QueryProcessor.h"

#include <stdexcept>
#include <utility>

#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

QP::QueryProcessor::QueryProcessor(PKB::StorageAccessInterface& pkb) : store(pkb), evaluator(store), post_processor(store) {}

std::vector<std::string> QP::QueryProcessor::processQuery(std::string query) {
	try {
		// Invalidate the cache at start of each query to prevent exceptions from leaving the cache in an inconsistent state.
		store.resetCache();
		QueryProperties query_properties = pre_processor.parseQuery(std::move(query));
		QueryResult result = evaluator.executeQuery(query_properties);
		return post_processor.processResult(query_properties, result);
	} catch (const QuerySemanticException& e) {
		return e.result;
	} catch (const std::exception& e) {
		return {};
	}
}

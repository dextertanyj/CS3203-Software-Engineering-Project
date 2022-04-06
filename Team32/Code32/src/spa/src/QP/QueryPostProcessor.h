#ifndef SPA_SRC_QP_QUERYFORMATTER_H
#define SPA_SRC_QP_QUERYFORMATTER_H

#include <string>

#include "QP/QP.h"
#include "QP/QueryProperties.h"
#include "QP/StorageAdapter.h"
#include "QueryResult.h"

class QP::QueryPostProcessor {
public:
	explicit QueryPostProcessor(const StorageAdapter& store);
	std::vector<std::string> processResult(QueryProperties& query_properties, QueryResult& query_result) const;

private:
	const StorageAdapter& store;
	std::vector<std::string> processStandardResult(QueryProperties& query_properties, QueryResult& query_result) const;
	static std::vector<std::string> processBooleanResult(QueryResult& query_result);
};

#endif  // SPA_SRC_QP_QUERYFORMATTER_H

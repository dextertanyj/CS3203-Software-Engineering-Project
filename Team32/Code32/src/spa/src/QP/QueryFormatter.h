#ifndef SPA_SRC_QP_QUERYFORMATTER_H
#define SPA_SRC_QP_QUERYFORMATTER_H

#include <string>
#include <vector>

#include "QP/QP.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"
#include "QP/StorageAdapter.h"

using namespace std;

class QP::QueryFormatter {
public:
	explicit QueryFormatter(const StorageAdapter& store);
	vector<string> formatResult(QueryProperties& query_properties, QueryResult& query_result) const;

private:
	const StorageAdapter& store;
	vector<string> formatBooleanResult(QueryResult& query_result) const;
	vector<string> formatNonBooleanResult(QueryProperties& query_properties, QueryResult& query_result) const;
};

#endif  // SPA_SRC_QP_QUERYFORMATTER_H

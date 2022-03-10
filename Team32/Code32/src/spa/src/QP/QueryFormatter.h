#ifndef SPA_SRC_QP_QUERYFORMATTER_H
#define SPA_SRC_QP_QUERYFORMATTER_H

#include <string>

#include "QP/QP.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

using namespace std;

class QP::QueryFormatter {
public:
	static vector<string> formatResult(QueryProperties& query_properties, QueryResult& query_result);

private:
	static vector<string> formatBooleanResult(QueryResult& query_result);
};

#endif  // SPA_SRC_QP_QUERYFORMATTER_H

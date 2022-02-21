#ifndef QUERY_FORMATTER_H_
#define QUERY_FORMATTER_H_

#include <string>

#include "QP/QueryProperties.h"
#include "QP/QP.h"
#include "QP/QueryResult.h"

using namespace std;

class QP::QueryFormatter {
public:
	static vector<string> formatResult(QueryProperties& query_properties, QueryResult& query_result);
};

#endif  // QUERY_FORMATTER_H_

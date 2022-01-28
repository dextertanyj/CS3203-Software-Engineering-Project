#ifndef QUERY_FORMATTER_H_
#define QUERY_FORMATTER_H_

#include <string>

#include "QueryProperties.h"
#include "QueryResult.h"

using namespace std;

class QueryFormatter {
public:
	static string formatQuery(const QueryProperties& queryProperties, const QueryResult& queryResult);
};

#endif // QUERY_FORMATTER_H_

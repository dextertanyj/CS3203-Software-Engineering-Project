#ifndef QUERY_FORMATTER_H_
#define QUERY_FORMATTER_H_

#include <string>

using namespace std;

class QueryProperties;
class QueryResult;

class QueryFormatter {
public:
	static string formatQuery(QueryProperties* queryProperties, QueryResult* queryResult);
};

#endif // QUERY_FORMATTER_H_

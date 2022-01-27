#pragma once

#include <string>

using namespace std;

class QueryProperties;
class QueryResult;

class QueryFormatter {
public:
	static string formatQuery(QueryProperties* queryProperties, QueryResult* queryResult);
};

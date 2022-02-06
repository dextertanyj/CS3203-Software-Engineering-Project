#ifndef QUERY_FORMATTER_H_
#define QUERY_FORMATTER_H_

#include <string>

#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

using namespace std;

class QueryFormatter {
public:
	static string formatResult(QueryProperties& queryProperties, QueryResult& queryResult);

private:
	static string stmtListToString(StmtRefList& stmts);
};

#endif // QUERY_FORMATTER_H_

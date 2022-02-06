#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_


#include <string>
#include <vector>

#include "QueryEvaluator.h"
#include "QueryFormatter.h"

using std::string;
using std::vector;

class QueryProcessor {
public:
	string processQuery(string query);

private:
	string formatResult();

	string query;
	QueryResult queryResult;
};

#endif // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_
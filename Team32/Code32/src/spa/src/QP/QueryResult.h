#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "QP/QueryTypeDefs.h"

class QueryResult {
public:
	QueryResult();
	bool getResult();
	vector<string> getSynonymResult(string synonym);
	void addColumn(string synonym, vector<string> column);
	void joinResult(QueryResult& queryResult);

private:
	bool result;
	unordered_map<string, vector<string>> table;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_

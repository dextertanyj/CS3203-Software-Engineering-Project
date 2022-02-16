#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "QP/QueryTypeDefs.h"

class QueryResult {
public:
	QueryResult();
	explicit QueryResult(bool result);
	bool getResult();
	unordered_map<string, vector<string>> getTable();
	unordered_set<string> getSynonymsStored();
	vector<string> getSynonymResult(string synonym);
	void addColumn(string synonym, vector<string> column);
	void joinResult(QueryResult& queryResult);

private:
	bool result;
	unordered_set<string> synonymsStored;
	unordered_map<string, vector<string>> table;
	void joinWithDifferentSynonym(QueryResult& queryResult);
	void joinWithSameSynonym(QueryResult& queryResult);
	bool contains(unordered_map<string, string> row);
	void removeRow(unordered_map<string, vector<string>> &table, int rowNumber);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_

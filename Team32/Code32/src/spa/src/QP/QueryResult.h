#ifndef SPA_SRC_QP_QUERYRESULT_H
#define SPA_SRC_QP_QUERYRESULT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"

class QP::QueryResult {
public:
	QueryResult();
	explicit QueryResult(bool result);
	[[nodiscard]] bool getResult() const;
	unordered_map<string, vector<string>> getTable();
	unordered_set<string> getSynonymsStored();
	vector<string> getSynonymResult(const string& synonym);
	void addColumn(const string& synonym, const vector<string>& column);
	void joinResult(QueryResult& query_result);

private:
	bool result;
	unordered_set<string> synonyms_stored;
	unordered_map<string, vector<string>> table;
	void joinWithDifferentSynonym(QueryResult& query_result);
	void joinWithSameSynonym(QueryResult& query_result);
	static bool contains(unordered_map<string, vector<string>>& table, const unordered_map<string, string>& row);
	static bool isRowMatch(const unordered_map<string, string>& row, unordered_map<string, vector<string>>& table, int row_number);
	static void removeRow(unordered_map<string, vector<string>>& table, int row_number);
	unordered_map<string, vector<string>> getSubTableWithRow(const unordered_map<string, string>& row);
};

#endif  // SPA_SRC_QP_QUERYRESULT_H

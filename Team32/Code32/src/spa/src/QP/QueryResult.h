#ifndef SPA_SRC_QP_QUERYRESULT_H
#define SPA_SRC_QP_QUERYRESULT_H

#include <queue>
#include <set>
#include <string>
#include <vector>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/ResultTable.h"
#include "QP/Types.h"

using QP::Types::ResultColumn;
using QP::Types::ResultRow;
using QP::Types::ResultTable;

class QP::QueryResult {
public:
	QueryResult();
	explicit QueryResult(bool result);
	explicit QueryResult(vector<string> synonyms);
	explicit QueryResult(ResultTable table);
	[[nodiscard]] bool getResult() const;
	ResultTable getTable();
	ResultColumn getSynonymResult(const string& synonym);
	ResultRow getRowWithOrder(const vector<string>& synonyms, size_t row_number);
	size_t getNumberOfRows();
	vector<string> getSynonymsStored();
	void addRow(const ResultRow& row);
	void filterBySelect(const QP::Types::DeclarationList& select_list);
	static QueryResult joinResults(vector<QueryResult>& result);
	static QueryResult joinIntraGroupResults(vector<QueryResult>& result);

private:
	bool result;
	ResultTable table;
	static bool compareLength(QueryResult result1, QueryResult result2);
	static unordered_map<string, vector<size_t>> getSynonymIndexMap(vector<QueryResult>& results);
	static void addNeighboursToQueue(const vector<size_t>& indexes, priority_queue<size_t, vector<size_t>, greater<>>& queue,
	                                 set<size_t>& completed);
};

#endif  // SPA_SRC_QP_QUERYRESULT_H

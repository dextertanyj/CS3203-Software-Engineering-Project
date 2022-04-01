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
	[[nodiscard]] ResultTable getTable() const;
	[[nodiscard]] ResultColumn getSynonymResult(const string& synonym) const;
	[[nodiscard]] ResultRow getRowWithOrder(const vector<string>& synonyms, size_t row_number) const;
	[[nodiscard]] size_t getNumberOfRows() const;
	[[nodiscard]] vector<string> getSynonymsStored() const;
	void addRow(const ResultRow& row);
	void filterBySelect(const QP::Types::DeclarationList& select_list);
	static QueryResult joinResults(vector<QueryResult>& result);
	static QueryResult joinIntraGroupResults(vector<QueryResult>& result);

private:
	bool result;
	ResultTable table;
	static bool compareLength(const QueryResult& lhs, const QueryResult& rhs);
	static unordered_map<string, vector<size_t>> getSynonymIndexMap(const vector<QueryResult>& results);
	static void findNeighbours(const QueryResult& current, unordered_map<string, vector<size_t>>& synonym_to_index_map,
	                           priority_queue<size_t, vector<size_t>, greater<>>& queue, unordered_set<size_t>& completed);
	static void addNeighboursToQueue(const vector<size_t>& indexes, priority_queue<size_t, vector<size_t>, greater<>>& queue,
	                                 unordered_set<size_t>& completed);
};

#endif  // SPA_SRC_QP_QUERYRESULT_H

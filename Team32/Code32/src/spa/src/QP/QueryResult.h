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

using QP::ResultTable;
using QP::Types::ResultColumn;
using QP::Types::ResultRow;

class QP::QueryResult {
public:
	QueryResult();
	explicit QueryResult(bool result);
	explicit QueryResult(std::vector<std::string> synonyms);
	explicit QueryResult(ResultTable table);
	[[nodiscard]] bool getResult() const;
	[[nodiscard]] bool containsSynonym(const std::string& synonym) const;
	[[nodiscard]] ResultTable getTable() const;
	[[nodiscard]] ResultColumn getSynonymResult(const std::string& synonym) const;
	[[nodiscard]] ResultRow getRowWithOrder(const std::vector<std::string>& synonyms, size_t row_number) const;
	[[nodiscard]] size_t getNumberOfRows() const;
	[[nodiscard]] std::vector<std::string> getSynonymsStored() const;
	void addRow(const ResultRow& row);
	void filterBySelect(const QP::Types::DeclarationList& select_list);
	static QueryResult joinResults(std::vector<QueryResult>& result);
	static QueryResult joinIntraGroupResults(std::vector<QueryResult>& result);

private:
	bool result;
	ResultTable table;
	static bool compareLength(const QueryResult& lhs, const QueryResult& rhs);
	static std::unordered_map<std::string, std::vector<size_t>> getSynonymToResultIndexMap(const std::vector<QueryResult>& results);
	static void findNeighbours(const QueryResult& current, std::unordered_map<std::string, std::vector<size_t>>& synonym_to_index_map,
	                           std::priority_queue<size_t, std::vector<size_t>, std::greater<>>& queue,
	                           std::unordered_set<size_t>& completed);
	static void addNeighboursToQueue(const std::vector<size_t>& indexes,
	                                 std::priority_queue<size_t, std::vector<size_t>, std::greater<>>& queue,
	                                 std::unordered_set<size_t>& completed);
};

#endif  // SPA_SRC_QP_QUERYRESULT_H

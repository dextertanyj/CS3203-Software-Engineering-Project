#ifndef SPA_SRC_QP_QUERYRESULT_H
#define SPA_SRC_QP_QUERYRESULT_H

#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/ResultTable.h"
#include "QP/Types.h"

class QP::QueryResult {
public:
	QueryResult();
	explicit QueryResult(bool result);
	explicit QueryResult(std::vector<std::string> synonyms);
	explicit QueryResult(ResultTable table);

	void addRow(const Types::ResultRow& row);
	void filterBySelect(const Types::DeclarationList& select_list);

	[[nodiscard]] bool getResult() const;
	[[nodiscard]] size_t getNumberOfRows() const;
	[[nodiscard]] bool containsSynonym(const std::string& synonym) const;
	[[nodiscard]] std::vector<std::string> getSynonymsStored() const;
	[[nodiscard]] Types::ResultColumn getSynonymResult(const std::string& synonym) const;
	[[nodiscard]] Types::ResultRow getRowWithOrder(const std::vector<std::string>& synonyms, size_t row_number) const;
	[[nodiscard]] ResultTable getTable() const;

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

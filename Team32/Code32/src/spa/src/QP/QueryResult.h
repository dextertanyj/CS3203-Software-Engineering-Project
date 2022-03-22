#ifndef SPA_SRC_QP_QUERYRESULT_H
#define SPA_SRC_QP_QUERYRESULT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/Types.h"

using QP::Types::ResultRow;
using QP::Types::ResultTable;

class QP::QueryResult {
public:
	QueryResult();
	explicit QueryResult(bool result);
	[[nodiscard]] bool getResult() const;
	ResultTable getTable();
	unordered_set<string> getSynonymsStored();
	vector<string> getSynonymResult(const string& synonym);
	size_t getTableSize();
	void addColumn(const string& synonym, const vector<string>& column);
	void joinResult(QueryResult& query_result);
	void filterBySelect(const QP::Types::DeclarationList& select_list);

private:
	bool result;
	unordered_set<string> synonyms_stored;
	ResultTable table;
	void joinWithDifferentSynonym(QueryResult& query_result);
	void joinWithSameSynonym(QueryResult& query_result);
	ResultTable getSubTableWithRow(const ResultRow& row);
	static bool contains(ResultTable& table, const ResultRow& row);
	static bool isRowMatch(const ResultRow& row, ResultTable& table, size_t row_number);
	static void removeRow(ResultTable& table, size_t row_number);
	static void removeDuplicateRows(ResultTable& table);
};

#endif  // SPA_SRC_QP_QUERYRESULT_H

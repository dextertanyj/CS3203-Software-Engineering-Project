#ifndef SPA_SRC_QP_RESULTTABLE_H
#define SPA_SRC_QP_RESULTTABLE_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Common/Hash.h"
#include "QP/QP.h"
#include "QP/Types.h"

using QP::Types::ResultColumn;
using QP::Types::ResultRow;

class QP::Types::ResultTable {
public:
	ResultTable();
	explicit ResultTable(vector<string> synonyms_stored);
	size_t getNumberOfRows();
	size_t getNumberOfColumns();
	vector<ResultRow> getTable();
	vector<string> getSynonymsStored();
	unordered_map<string, size_t> getSynonymsStoredMap();
	ResultColumn getColumn(const string& synonym);
	ResultRow getRow(size_t row_number);
	ResultRow getRowWithOrder(const vector<string>& synonyms, size_t row_number);
	void insertRow(const ResultRow& row);
	void removeRow(size_t row_number);
	ResultTable filterBySelect(const QP::Types::DeclarationList& select_list);
	bool containsRow(const ResultRow& row);
	static ResultTable joinTables(ResultTable table_one, ResultTable table_two);

private:
	unordered_map<string, size_t> synonyms_to_index_map;
	vector<string> synonyms_stored;
	vector<ResultRow> table;

	static ResultTable intersectTables(ResultTable superset_table, const ResultTable& subset_table);
	static ResultTable hashJoinTables(ResultTable table_one, ResultTable table_two);
	static ResultTable loopJoinTables(const ResultTable& table_one, const ResultTable& table_two);
	static unordered_multimap<ResultRow, size_t> buildHashTable(ResultTable& table, const vector<string>& key_synonyms);
};

#endif  // SPA_SRC_QP_RESULTTABLE_H

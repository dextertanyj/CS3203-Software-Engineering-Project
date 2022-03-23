#ifndef SPA_SRC_QP_RESULTTABLE_H
#define SPA_SRC_QP_RESULTTABLE_H

#include <map>
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
	ResultTable(vector<string> synonyms_stored);
	size_t getNumberOfRows();
	size_t getNumberOfColumns();
	vector<ResultRow> getTable();
	vector<string> getSynonymsStored();
	unordered_map<string, size_t> getSynonymsStoredMap();
	ResultColumn getColumn(const string& synonym);
	ResultRow getRow(size_t row_number);
	void insertRow(const ResultRow& row);
	void insertColumn(const string& synonym, const ResultColumn& column);
	ResultTable filterBySelect(const QP::Types::DeclarationList& select_list);
	static ResultTable joinTables(ResultTable& table_one, ResultTable& table_two);

private:
	unordered_map<string, size_t> synonyms_to_index_map;
	vector<string> synonyms_stored;
	vector<ResultRow> table;

	unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> buildHashTable(vector<string> synonyms);
	void removeRow(size_t row_number);
	void removeDuplicateRows();
	ResultRow getSubRow(vector<string> synonyms, size_t row_number);
	static ResultTable intersectTables(ResultTable& larger_table, ResultTable& smaller_table);
	static ResultTable crossJoinTables(ResultTable& table_one, ResultTable& table_two);
};

#endif  // SPA_SRC_QP_RESULTTABLE_H

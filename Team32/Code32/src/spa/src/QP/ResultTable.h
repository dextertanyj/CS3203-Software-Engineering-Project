#ifndef SPA_SRC_QP_RESULTTABLE_H
#define SPA_SRC_QP_RESULTTABLE_H

#include <vector>
#include <utility>

#include "QP/QP.h"
#include "QP/QueryTypes.h"

using QP::Types::ResultColumn;
using QP::Types::ResultRow;

class QP::Types::ResultTable {
public:
	ResultTable();
	ResultTable(vector<string> synonyms_stored);
	ResultTable(vector<string> synonyms_stored, vector<ResultRow> table);
	size_t getNumberOfRows();
	size_t getNumberOfColumns();
	vector<ResultRow> getTable();
	unordered_map<string, size_t> getSynonymsStored();
	ResultColumn getColumn(const string& synonym);
	void insertRow(const ResultRow& row);
	void insertColumn(const string& synonym, const ResultColumn& column);
	void filterBySelect(const QP::Types::DeclarationList& select_list);
	static ResultTable joinTables(pair<ResultTable&, ResultTable&> tables);

private:
	unordered_map<string, size_t> synonyms_stored;
	vector<ResultRow> table;

	bool contains(const unordered_map<string, string>& row);
	bool isRowMatch(const unordered_map<string, string>& sub_row, size_t row_number);
	void removeRow(size_t row_number);
	void removeColumn(const string& synonym);
	void removeDuplicateRows();
	ResultTable getSubTableWithRow(const ResultRow& row);
	static ResultTable joinWithSameSynonym(ResultTable& larger_table, ResultTable& smaller_table);
	static ResultTable joinWithDifferentSynonym(ResultTable& larger_table, ResultTable& smaller_table);
};

#endif  // SPA_SRC_QP_RESULTTABLE_H

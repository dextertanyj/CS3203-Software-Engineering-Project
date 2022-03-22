#ifndef SPA_SRC_QP_RESULTTABLE_H
#define SPA_SRC_QP_RESULTTABLE_H

#include "QP/QP.h"
#include "QP/QueryTypes.h"

using QP::Types::ResultColumn;
using QP::Types::ResultRow;

class QP::Types::ResultTable {
public:
	ResultTable();
	ResultTable(unordered_set<string> synonyms_stored, unordered_map<string, ResultColumn> table);
	size_t getNumberOfRows();
	size_t getNumberOfColumns();
	unordered_map<string, vector<string>> getTable();
	unordered_set<string> getSynonymsStored();
	ResultColumn getColumn(const string& synonym);
	void insertColumn(const string& synonym, const ResultColumn& column);
	void filterBySelect(const QP::Types::DeclarationList& select_list);
	static ResultTable joinTables(pair<ResultTable&, ResultTable&> tables);

private:
	unordered_set<string> synonyms_stored;
	unordered_map<string, ResultColumn> table;

	ResultTable getSubTableWithRow(const ResultRow& row);
	bool contains(const ResultRow& row);
	bool isRowMatch(const ResultRow& row, size_t row_number);
	void removeRow(size_t row_number);
	void removeDuplicateRows();
	static ResultTable joinWithSameSynonym(ResultTable& larger_table, ResultTable& smaller_table);
	static ResultTable joinWithDifferentSynonym(ResultTable& larger_table, ResultTable& smaller_table);
};

#endif  // SPA_SRC_QP_RESULTTABLE_H

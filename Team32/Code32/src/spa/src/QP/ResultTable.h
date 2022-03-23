#ifndef SPA_SRC_QP_RESULTTABLE_H
#define SPA_SRC_QP_RESULTTABLE_H

#include <map>
#include <vector>
#include <utility>

#include "QP/QP.h"
#include "QP/Types.h"
#include "Common/Hash.h"

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
	static ResultTable joinTables(pair<ResultTable&, ResultTable&> tables);

private:
	unordered_map<string, size_t> synonyms_to_index_map;
	vector<string> synonyms_stored;
	vector<ResultRow> table;

	unordered_multimap<ResultRow, int, Common::Hash::VectorHash> buildHashTable(vector<string> synonyms);
	bool contains(const unordered_map<string, string>& row);
	bool isRowMatch(const unordered_map<string, string>& sub_row, size_t row_number);
	void removeRow(size_t row_number);
	void removeColumn(const string& synonym);
	void removeDuplicateRows();
	ResultRow getSubRow(vector<string> synonyms, size_t row_number);
	static ResultTable joinWithSameSynonym(pair<ResultTable&, ResultTable&> tables);
	static ResultTable joinWithDifferentSynonym(pair<ResultTable&, ResultTable&> tables);
};

#endif  // SPA_SRC_QP_RESULTTABLE_H

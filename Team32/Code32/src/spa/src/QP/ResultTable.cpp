#include "ResultTable.h"

#include <algorithm>
#include <cassert>
#include <unordered_set>

using QP::Types::ResultTable;

QP::Types::ResultTable::ResultTable() = default;

QP::Types::ResultTable::ResultTable(vector<string> synonyms_stored) : synonyms_stored(synonyms_stored) {
	for (size_t i = 0; i < synonyms_stored.size(); i++) {
		this->synonyms_to_index_map.insert({synonyms_stored[i], i});
	}
}

size_t QP::Types::ResultTable::getNumberOfRows() { return table.size(); }

size_t QP::Types::ResultTable::getNumberOfColumns() { return synonyms_stored.size(); }

vector<ResultRow> QP::Types::ResultTable::getTable() { return table; }

vector<string> QP::Types::ResultTable::getSynonymsStored() { return synonyms_stored; }

unordered_map<string, size_t> QP::Types::ResultTable::getSynonymsStoredMap() { return synonyms_to_index_map; }

ResultColumn QP::Types::ResultTable::getColumn(const string& synonym) {
	assert(synonyms_to_index_map.find(synonym) != synonyms_to_index_map.end());

	size_t col_pos = synonyms_to_index_map.at(synonym);
	size_t number_of_rows = getNumberOfRows();
	ResultColumn column(number_of_rows);
	for (int i = 0; i < number_of_rows; i++) {
		column[i] = getRow(i)[col_pos];
	}
	return column;
}

ResultRow QP::Types::ResultTable::getRow(size_t row_number) { return table.at(row_number); }

void QP::Types::ResultTable::insertRow(const ResultRow& row) {
	assert(row.size() == getNumberOfColumns());

	table.push_back(row);
}

QP::Types::ResultTable QP::Types::ResultTable::filterBySelect(const QP::Types::DeclarationList& select_list) {
	vector<string> synonyms(select_list.size());
	transform(select_list.begin(), select_list.end(), synonyms.begin(), [](const Declaration& declaration) { return declaration.symbol; });

	ResultTable filtered_table = ResultTable(synonyms);

	unordered_set<ResultRow, Common::Hash::VectorHash> rows;
	for (auto const& row : table) {
		ResultRow sub_row(select_list.size());
		for (int i = 0; i < select_list.size(); i++) {
			sub_row[i] = row.at(synonyms_to_index_map.at(select_list[i].symbol));
		}
		rows.insert(sub_row);
	}

	for (auto const& row : rows) {
		filtered_table.insertRow(row);
	}

	return filtered_table;
}

bool QP::Types::ResultTable::containsRow(const ResultRow& row) {
	for (auto const& row_stored : table) {
		if (row_stored == row) {
			return true;
		}
	}

	return false;
}

void QP::Types::ResultTable::removeRow(size_t row_number) {
	// Narrowing conversion, implementation defined behaviour for values greater than long.
	table.erase(table.begin() + static_cast<vector<string>::difference_type>(row_number));
}

ResultRow QP::Types::ResultTable::getRowWithOrder(const vector<string>& synonyms, size_t row_number) {
	ResultRow sub_row;
	ResultRow row = table.at(row_number);
	for (string const& synonym : synonyms) {
		sub_row.push_back(row.at(synonyms_to_index_map.at(synonym)));
	}
	return sub_row;
}

static unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> buildHashTable(ResultTable& table, const vector<string>& synonyms) {
	unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> map;
	unordered_map<string, size_t> synonyms_to_index_map = table.getSynonymsStoredMap();
	size_t row_number = 0;
	for (ResultRow const& row : table.getTable()) {
		ResultRow sub_row;
		for (string const& synonym : synonyms) {
			sub_row.push_back(row.at(synonyms_to_index_map.at(synonym)));
		}
		map.insert({sub_row, row_number});
		row_number++;
	}

	return map;
}

static QP::Types::ResultTable intersectTables(ResultTable superset_table, ResultTable subset_table) {
	vector<string> common_synonyms = subset_table.getSynonymsStored();
	unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> hashmap = buildHashTable(subset_table, common_synonyms);

	size_t number_of_rows = superset_table.getNumberOfRows();
	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		auto range = hashmap.equal_range(superset_table.getRowWithOrder(common_synonyms, pos));
		if (range.first == range.second) {
			superset_table.removeRow(pos);
		} else {
			pos++;
		}
	}

	return superset_table;
}

static ResultRow buildRow(ResultRow current_row, ResultRow new_row, const vector<string>& synonyms,
                          const unordered_map<string, size_t>& map) {
	for (string const& synonym : synonyms) {
		size_t index = map.at(synonym);
		current_row.push_back(new_row.at(index));
	}
	return current_row;
}

static QP::Types::ResultTable crossJoinTables(ResultTable table_one, ResultTable table_two) {
	ResultTable larger_table;
	ResultTable smaller_table;
	if (table_one.getNumberOfRows() >= table_two.getNumberOfRows()) {
		larger_table = table_one;
		smaller_table = table_two;
	} else {
		larger_table = table_two;
		smaller_table = table_one;
	}

	vector<string> common_synonyms;
	vector<string> new_synonyms;
	unordered_map<string, size_t> current_synonyms = larger_table.getSynonymsStoredMap();
	for (auto const& synonym : smaller_table.getSynonymsStored()) {
		if (current_synonyms.find(synonym) != current_synonyms.end()) {
			common_synonyms.push_back(synonym);
		} else {
			new_synonyms.push_back(synonym);
		}
	}

	vector<string> final_synonyms = larger_table.getSynonymsStored();
	final_synonyms.insert(final_synonyms.end(), new_synonyms.begin(), new_synonyms.end());
	ResultTable final_table = ResultTable(final_synonyms);

	unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> hashmap = buildHashTable(smaller_table, common_synonyms);
	for (size_t i = 0; i < larger_table.getNumberOfRows(); i++) {
		auto range = hashmap.equal_range(larger_table.getRowWithOrder(common_synonyms, i));
		for (auto it = range.first; it != range.second; it++) {
			ResultRow row =
				buildRow(larger_table.getRow(i), smaller_table.getRow(it->second), new_synonyms, smaller_table.getSynonymsStoredMap());
			final_table.insertRow(row);
		}
	}

	return final_table;
}

QP::Types::ResultTable QP::Types::ResultTable::joinTables(ResultTable table_one, ResultTable table_two) {
	ResultTable superset_table;
	ResultTable subset_table;

	if (table_one.getNumberOfColumns() >= table_two.getNumberOfColumns()) {
		superset_table = table_one;
		subset_table = table_two;
	} else {
		superset_table = table_two;
		subset_table = table_one;
	}

	unordered_map<string, size_t> superset_synonyms = superset_table.getSynonymsStoredMap();
	for (auto const& synonym : subset_table.getSynonymsStored()) {
		if (superset_synonyms.find(synonym) == superset_synonyms.end()) {
			return crossJoinTables(superset_table, subset_table);
		}
	}

	return intersectTables(superset_table, subset_table);
}

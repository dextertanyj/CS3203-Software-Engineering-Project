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

	unordered_set<ResultRow> rows;
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
	return any_of(table.begin(), table.end(), [row](auto const& row_stored) { return row_stored == row; });
}

void QP::Types::ResultTable::removeRow(size_t row_number) {
	// Narrowing conversion, implementation defined behaviour for values greater than long.
	table.erase(table.begin() + static_cast<vector<string>::difference_type>(row_number));
}

ResultRow QP::Types::ResultTable::getRowWithOrder(const vector<string>& synonyms, size_t row_number) {
	ResultRow row_with_order;
	ResultRow row = table.at(row_number);
	for (string const& synonym : synonyms) {
		row_with_order.push_back(row.at(synonyms_to_index_map.at(synonym)));
	}
	return row_with_order;
}

unordered_multimap<ResultRow, size_t> QP::Types::ResultTable::buildHashTable(ResultTable& table, const vector<string>& key_synonyms) {
	unordered_multimap<ResultRow, size_t> map;
	unordered_map<string, size_t> synonyms_to_index_map = table.getSynonymsStoredMap();
	size_t row_number = 0;
	for (ResultRow const& row : table.table) {
		ResultRow sub_row;
		sub_row.reserve(key_synonyms.size());
		for (string const& synonym : key_synonyms) {
			sub_row.push_back(row.at(synonyms_to_index_map.at(synonym)));
		}
		map.insert({sub_row, row_number});
		row_number++;
	}

	return map;
}

QP::Types::ResultTable QP::Types::ResultTable::intersectTables(ResultTable superset_table, const ResultTable& subset_table) {
	vector<string> common_synonyms = subset_table.synonyms_stored;
	vector<ResultRow> table = subset_table.table;
	unordered_set<ResultRow> record_set(table.begin(), table.end());

	size_t number_of_rows = superset_table.getNumberOfRows();
	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		if (record_set.find(superset_table.getRowWithOrder(common_synonyms, pos)) == record_set.end()) {
			superset_table.removeRow(pos);
		} else {
			pos++;
		}
	}

	return superset_table;
}

static ResultRow mergeRow(ResultRow current_row, const ResultRow& other_row, const vector<string>& synonym_order,
                          const unordered_map<string, size_t>& synonym_map) {
	for (string const& synonym : synonym_order) {
		size_t index = synonym_map.at(synonym);
		current_row.push_back(other_row.at(index));
	}
	return current_row;
}

QP::Types::ResultTable QP::Types::ResultTable::hashJoinTables(ResultTable table_one, ResultTable table_two) {
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

	unordered_multimap<ResultRow, size_t> map = buildHashTable(smaller_table, common_synonyms);
	for (size_t i = 0; i < larger_table.getNumberOfRows(); i++) {
		auto range = map.equal_range(larger_table.getRowWithOrder(common_synonyms, i));
		for (auto it = range.first; it != range.second; it++) {
			ResultRow row =
				mergeRow(larger_table.table.at(i), smaller_table.table.at(it->second), new_synonyms, smaller_table.synonyms_to_index_map);
			final_table.insertRow(row);
		}
	}

	return final_table;
}

QP::Types::ResultTable QP::Types::ResultTable::loopJoinTables(const ResultTable& table_one, const ResultTable& table_two) {
	vector<string> final_synonyms;
	final_synonyms.insert(final_synonyms.end(), table_one.synonyms_stored.begin(), table_one.synonyms_stored.end());
	final_synonyms.insert(final_synonyms.end(), table_two.synonyms_stored.begin(), table_two.synonyms_stored.end());

	ResultTable table = ResultTable(final_synonyms);
	for (auto const& table_one_row : table_one.table) {
		for (auto const& table_two_row : table_two.table) {
			ResultRow row = table_one_row;
			row.insert(row.end(), table_two_row.begin(), table_two_row.end());
			table.insertRow(row);
		}
	}

	return table;
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
	vector<string> subset_synonyms = subset_table.getSynonymsStored();

	size_t number_of_match = count_if(subset_synonyms.begin(), subset_synonyms.end(), [&](auto const& synonym) {
		return superset_synonyms.find(synonym) != superset_synonyms.end();
	});

	if (number_of_match == subset_synonyms.size()) {
		return intersectTables(move(superset_table), subset_table);
	}

	if (number_of_match > 0) {
		return hashJoinTables(move(superset_table), move(subset_table));
	}

	return loopJoinTables(superset_table, subset_table);
}

#include "QP/ResultTable.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <unordered_set>

using namespace std;
using namespace QP::Types;

QP::ResultTable::ResultTable() = default;

QP::ResultTable::ResultTable(vector<string> synonyms_stored) : synonyms_stored(synonyms_stored) {
	this->synonyms_to_index_map.reserve(synonyms_stored.size());
	for (size_t i = 0; i < synonyms_stored.size(); i++) {
		this->synonyms_to_index_map.emplace(synonyms_stored[i], i);
	}
}

void QP::ResultTable::insertRow(const ResultRow& row) {
	assert(row.size() == getNumberOfColumns());
	table.push_back(row);
}

QP::ResultTable QP::ResultTable::filterBySelect(const DeclarationList& select_list) {
	vector<string> synonyms;
	synonyms.reserve(select_list.size());
	transform(select_list.begin(), select_list.end(), back_inserter(synonyms),
	          [](const Declaration& declaration) { return declaration.symbol; });

	ResultTable filtered_table = ResultTable(synonyms);

	unordered_set<ResultRow> rows;
	rows.reserve(getNumberOfRows());
	for (const auto& row : table) {
		ResultRow sub_row;
		sub_row.reserve(select_list.size());
		for (const auto& select : select_list) {
			sub_row.push_back(row[synonyms_to_index_map.at(select.symbol)]);
		}
		rows.emplace(sub_row);
	}

	for (const auto& row : rows) {
		filtered_table.insertRow(row);
	}

	return filtered_table;
}

size_t QP::ResultTable::getNumberOfRows() const { return table.size(); }

size_t QP::ResultTable::getNumberOfColumns() const { return synonyms_stored.size(); }

vector<string> QP::ResultTable::getSynonymsStored() const { return synonyms_stored; }

unordered_map<string, size_t> QP::ResultTable::getSynonymsStoredMap() const { return synonyms_to_index_map; }

bool QP::ResultTable::containsRow(const ResultRow& row) const {
	return any_of(table.begin(), table.end(), [&](const auto& row_stored) { return row_stored == row; });
}

ResultRow QP::ResultTable::getRow(size_t row_number) const { return table.at(row_number); }

ResultRow QP::ResultTable::getRowWithOrder(const vector<string>& synonyms, size_t row_number) const {
	ResultRow row_with_order;
	row_with_order.reserve(synonyms.size());
	ResultRow row = table.at(row_number);
	for (const auto& synonym : synonyms) {
		row_with_order.push_back(row[synonyms_to_index_map.at(synonym)]);
	}
	return row_with_order;
}

ResultColumn QP::ResultTable::getColumn(const string& synonym) const {
	assert(synonyms_to_index_map.find(synonym) != synonyms_to_index_map.end());

	size_t col_pos = synonyms_to_index_map.at(synonym);
	size_t number_of_rows = getNumberOfRows();
	ResultColumn column(number_of_rows);
	for (int i = 0; i < number_of_rows; i++) {
		column[i] = getRow(i)[col_pos];
	}
	return column;
}

vector<ResultRow> QP::ResultTable::getTable() const { return table; }

QP::ResultTable QP::ResultTable::joinTables(const ResultTable& table_one, const ResultTable& table_two) {
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

	size_t number_of_match = count_if(subset_synonyms.begin(), subset_synonyms.end(),
	                                  [&](const auto& synonym) { return superset_synonyms.find(synonym) != superset_synonyms.end(); });

	if (number_of_match == subset_synonyms.size()) {
		return intersectTables(superset_table, subset_table);
	}

	if (number_of_match > 0) {
		return hashJoinTables(superset_table, subset_table);
	}

	return loopJoinTables(superset_table, subset_table);
}

QP::ResultTable QP::ResultTable::intersectTables(const ResultTable& superset_table, const ResultTable& subset_table) {
	vector<string> common_synonyms = subset_table.synonyms_stored;
	vector<ResultRow> table = subset_table.table;
	unordered_set<ResultRow> record_set(table.begin(), table.end());

	ResultTable result = ResultTable(superset_table.getSynonymsStored());

	size_t number_of_rows = superset_table.getNumberOfRows();
	for (size_t i = 0; i < number_of_rows; i++) {
		auto probe_row = superset_table.getRowWithOrder(common_synonyms, i);
		if (record_set.find(probe_row) == record_set.end()) {
			continue;
		}
		result.insertRow(superset_table.getRow(i));
	}

	return result;
}

static ResultRow mergeRow(ResultRow current_row, const ResultRow& other_row, const vector<string>& synonym_order,
                          const unordered_map<string, size_t>& synonym_map) {
	current_row.reserve(current_row.size() + synonym_order.size());
	for (const auto& synonym : synonym_order) {
		size_t index = synonym_map.at(synonym);
		current_row.push_back(other_row[index]);
	}
	return current_row;
}

QP::ResultTable QP::ResultTable::hashJoinTables(const ResultTable& table_one, const ResultTable& table_two) {
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
	for (const auto& synonym : smaller_table.getSynonymsStored()) {
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
			auto outer_row = larger_table.table[i];
			auto inner_row = smaller_table.table[it->second];
			ResultRow row = mergeRow(outer_row, inner_row, new_synonyms, smaller_table.synonyms_to_index_map);
			final_table.insertRow(row);
		}
	}

	return final_table;
}

QP::ResultTable QP::ResultTable::loopJoinTables(const ResultTable& table_one, const ResultTable& table_two) {
	vector<string> final_synonyms;
	final_synonyms.reserve(table_one.synonyms_stored.size() + table_two.synonyms_stored.size());
	final_synonyms.insert(final_synonyms.end(), table_one.synonyms_stored.begin(), table_one.synonyms_stored.end());
	final_synonyms.insert(final_synonyms.end(), table_two.synonyms_stored.begin(), table_two.synonyms_stored.end());

	ResultTable table = ResultTable(final_synonyms);
	for (const auto& table_one_row : table_one.table) {
		for (const auto& table_two_row : table_two.table) {
			ResultRow row = table_one_row;
			row.reserve(table_two_row.size());
			row.insert(row.end(), table_two_row.begin(), table_two_row.end());
			table.insertRow(row);
		}
	}

	return table;
}

unordered_multimap<ResultRow, size_t> QP::ResultTable::buildHashTable(ResultTable& table, const vector<string>& key_synonyms) {
	unordered_multimap<ResultRow, size_t> map;
	unordered_map<string, size_t> synonyms_to_index_map = table.getSynonymsStoredMap();
	size_t row_number = 0;
	for (const auto& row : table.table) {
		ResultRow sub_row;
		sub_row.reserve(key_synonyms.size());
		for (const auto& synonym : key_synonyms) {
			sub_row.push_back(row.at(synonyms_to_index_map.at(synonym)));
		}
		map.emplace(sub_row, row_number);
		row_number++;
	}

	return map;
}

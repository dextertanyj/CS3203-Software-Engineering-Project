#include "ResultTable.h"

#include <algorithm>
#include <cassert>

QP::Types::ResultTable::ResultTable() {}

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
	try {
		size_t col_pos = synonyms_to_index_map.at(synonym);
		size_t number_of_rows = getNumberOfRows();
		ResultColumn column(number_of_rows);
		for (int i = 0; i < number_of_rows; i++) {
			column[i] = getRow(i)[col_pos];
		}
		return column;
	} catch (const std::out_of_range& e) {
		return {};
	}
}

ResultRow QP::Types::ResultTable::getRow(size_t row_number) { return table.at(row_number); }

void QP::Types::ResultTable::insertRow(const ResultRow& row) {
	assert(row.size() == getNumberOfColumns);

	table.push_back(row);
}

void QP::Types::ResultTable::insertColumn(const string& synonym, const ResultColumn& column) {
	synonyms_to_index_map.insert({synonym, getNumberOfColumns()});
	synonyms_stored.push_back(synonym);

	if (getNumberOfColumns() == 1) {
		for (string const& value : column) {
			ResultRow row;
			row.push_back(value);
			table.push_back(row);
		}
		return;
	}

	for (int i = 0; i < getNumberOfRows(); i++) {
		ResultRow row = getRow(i);
		row.push_back(column[i]);
		table.at(i) = row;
	}
}

QP::Types::ResultTable QP::Types::ResultTable::filterBySelect(const QP::Types::DeclarationList& select_list) {
	ResultTable filtered_table;

	for (auto const& declaration : select_list) {
		filtered_table.insertColumn(declaration.symbol, getColumn(declaration.symbol));
	}

	filtered_table.removeDuplicateRows();
	return filtered_table;
}

QP::Types::ResultTable QP::Types::ResultTable::joinTables(ResultTable& table_one, ResultTable& table_two) {
	ResultTable larger_table;
	ResultTable smaller_table;

	if (table_one.getNumberOfColumns() >= table_two.getNumberOfColumns()) {
		larger_table = table_one;
		smaller_table = table_two;
	} else {
		larger_table = table_two;
		smaller_table = table_one;
	}

	unordered_map<string, size_t> larger_set = larger_table.getSynonymsStoredMap();
	for (auto const& synonym : smaller_table.getSynonymsStored()) {
		if (larger_set.find(synonym) == larger_set.end()) {
			return crossJoinTables(larger_table, smaller_table);
		}
	}

	return intersectTables(larger_table, smaller_table);
}

unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> QP::Types::ResultTable::buildHashTable(vector<string> synonyms) {
	unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> map;
	size_t row_number = 0;
	for (ResultRow const& row : table) {
		ResultRow sub_row;
		for (string const& synonym : synonyms) {
			sub_row.push_back(row.at(synonyms_to_index_map.at(synonym)));
		}
		map.insert({sub_row, row_number});
		row_number++;
	}

	return map;
}

void QP::Types::ResultTable::removeRow(size_t row_number) { table.erase(table.begin() + row_number); }

void QP::Types::ResultTable::removeDuplicateRows() {
	unordered_set<ResultRow, Common::Hash::VectorHash> rows;
	size_t number_of_rows = getNumberOfRows();

	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		ResultRow row = getRow(pos);
		if (rows.find(row) != rows.end()) {
			removeRow(pos);
		} else {
			pos++;
			rows.insert(row);
		}
	}
}

ResultRow QP::Types::ResultTable::getSubRow(vector<string> synonyms, size_t row_number) {
	ResultRow sub_row;
	ResultRow row = table.at(row_number);
	for (string const& synonym : synonyms) {
		sub_row.push_back(row.at(synonyms_to_index_map.at(synonym)));
	}
	return sub_row;
}

QP::Types::ResultTable QP::Types::ResultTable::intersectTables(ResultTable& larger_table, ResultTable& smaller_table) {
	vector<string> common_synonyms = smaller_table.getSynonymsStored();
	unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> hashmap = smaller_table.buildHashTable(common_synonyms);

	size_t number_of_rows = larger_table.getNumberOfRows();
	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		auto range = hashmap.equal_range(larger_table.getSubRow(common_synonyms, pos));
		if (range.first == range.second) {
			larger_table.removeRow(pos);
		} else {
			pos++;
		}
	}

	return larger_table;
}

QP::Types::ResultTable QP::Types::ResultTable::crossJoinTables(ResultTable& table_one, ResultTable& table_two) {
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

	unordered_multimap<ResultRow, size_t, Common::Hash::VectorHash> hashmap = smaller_table.buildHashTable(common_synonyms);
	for (size_t i = 0; i < larger_table.getNumberOfRows(); i++) {
		auto range = hashmap.equal_range(larger_table.getSubRow(common_synonyms, i));
		for (auto it = range.first; it != range.second; it++) {
			ResultRow row = larger_table.getRow(i);
			for (string const& new_syn : new_synonyms) {
				size_t index = smaller_table.getSynonymsStoredMap().at(new_syn);
				row.push_back(smaller_table.getRow(it->second).at(index));
			}
			final_table.insertRow(row);
		}
	}

	return final_table;
}

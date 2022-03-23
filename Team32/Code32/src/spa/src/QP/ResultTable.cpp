#include "ResultTable.h"

#include <cassert>
#include <iostream>

QP::Types::ResultTable::ResultTable() {}

QP::Types::ResultTable::ResultTable(vector<string> synonyms_stored) {
	for (size_t i = 0; i < synonyms_stored.size(); i++) {
		this->synonyms_stored.insert({synonyms_stored[i], i});
	}
}

QP::Types::ResultTable::ResultTable(vector<string> synonyms_stored, vector<ResultRow> table) : table(table) {
	for (size_t i = 0; i < synonyms_stored.size(); i++) {
		this->synonyms_stored.insert({synonyms_stored[i], i});
	}
}

size_t QP::Types::ResultTable::getNumberOfRows() {
	return table.size();
}

size_t QP::Types::ResultTable::getNumberOfColumns() { return synonyms_stored.size(); }

vector<ResultRow> QP::Types::ResultTable::getTable() { return table; }

unordered_map<string, size_t> QP::Types::ResultTable::getSynonymsStored() {
	return synonyms_stored;
}

ResultColumn QP::Types::ResultTable::getColumn(const string& synonym) {
	try {
		size_t col_pos = synonyms_stored.at(synonym);
		vector<string> column(getNumberOfRows());
		for (auto const& row : table) {
			column.push_back(row[col_pos]);
		}
		return column;
	} catch (const std::out_of_range& e) {
		return {};
	}
}

void QP::Types::ResultTable::insertRow(const ResultRow& row) {
	assert(row.size() == getNumberOfColumns);

	table.push_back(row);
}

void QP::Types::ResultTable::insertColumn(const string& synonym, const ResultColumn& column) {
	synonyms_stored.insert({synonym, getNumberOfColumns()});

	size_t pos = 0;
	for (auto row : table) {
		row.push_back(column[pos]);
		pos++;
	}
}

void QP::Types::ResultTable::filterBySelect(const QP::Types::DeclarationList& select_list) {
	for (auto const& declaration : select_list) {
		removeColumn(declaration.symbol);
	}

	removeDuplicateRows();
}

QP::Types::ResultTable QP::Types::ResultTable::joinTables(pair<ResultTable&, ResultTable&> tables) {
	ResultTable larger_table;
	ResultTable smaller_table;

	if (tables.first.getNumberOfColumns() >= tables.second.getNumberOfColumns()) {
		larger_table = tables.first;
		smaller_table = tables.second;
	} else {
		larger_table = tables.second;
		smaller_table = tables.first;
	}

	unordered_map<string, size_t> larger_set = larger_table.getSynonymsStored();
	for (auto const& pair : smaller_table.getSynonymsStored()) {
		if (larger_set.find(pair.first) == larger_set.end()) {
			return joinWithDifferentSynonym(larger_table, smaller_table);
		}
	}

	return joinWithSameSynonym(larger_table, smaller_table);
}

bool QP::Types::ResultTable::contains(const unordered_map<string, string>& row) {
	size_t number_of_rows = getNumberOfRows();
	for (size_t i = 0; i < number_of_rows; i++) {
		if (isRowMatch(row, i)) {
			return true;
		}
	}

	return false;
}

bool QP::Types::ResultTable::isRowMatch(const unordered_map<string, string>& sub_row, size_t row_number) {
	bool has_match = true;
	ResultRow& row = table.at(row_number);
	for (const auto& iterator : sub_row) {
		if (row.at(synonyms_stored.at(iterator.first)) != iterator.second) {
			has_match = false;
			break;
		}
	}

	return has_match;
}

void QP::Types::ResultTable::removeRow(size_t row_number) {
	table.erase(table.begin() + row_number);
}

void QP::Types::ResultTable::removeColumn(const string& synonym) {
	size_t col_number = synonyms_stored.at(synonym);
	synonyms_stored.erase(synonym);
	size_t pos = 0;
	for (auto row : table) {
		row.erase(row.begin() + col_number);
		pos++;
	}
}

void QP::Types::ResultTable::removeDuplicateRows() {
	unordered_set<string> rows;
	size_t number_of_rows = getNumberOfRows();

	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		string row_string;
		for (auto const& value : table.at(i)) {
			row_string.append(value);
			row_string.append(" ");
		}

		if (rows.find(row_string) != rows.end()) {
			removeRow(pos);
		} else {
			pos++;
			rows.insert(row_string);
		}
	}
}

QP::Types::ResultTable QP::Types::ResultTable::joinWithSameSynonym(ResultTable& larger_table, ResultTable& smaller_table) {
	size_t number_of_rows = larger_table.getNumberOfRows();
	unordered_map<string, ResultColumn> temp = larger_table.getTable();
	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		ResultRow row;
		for (const string& synonym : smaller_table.getSynonymsStored()) {
			row.insert({synonym, temp.at(synonym)[i]});
		}

		if (!smaller_table.contains(row)) {
			larger_table.removeRow(pos);
		} else {
			pos++;
		}
	}

	return larger_table;
}

QP::Types::ResultTable QP::Types::ResultTable::joinWithDifferentSynonym(ResultTable& larger_table, ResultTable& smaller_table) {
	unordered_set<string> old_synonyms;
	unordered_set<string> new_synonyms;
	unordered_set<string> common_synonyms;
	unordered_map<string, ResultColumn> new_table;

	unordered_set<string> larger_set = larger_table.getSynonymsStored();
	unordered_set<string> smaller_set = smaller_table.getSynonymsStored();
	for (string const& synonym : smaller_set) {
		new_table.insert({synonym, {}});
		if (larger_set.find(synonym) != larger_set.end()) {
			common_synonyms.insert(synonym);
		} else {
			new_synonyms.insert(synonym);
		}
	}

	for (string const& old_synonym : larger_set) {
		if (common_synonyms.find(old_synonym) == common_synonyms.end()) {
			new_table.insert({old_synonym, {}});
			old_synonyms.insert(old_synonym);
		}
	}

	size_t number_of_rows = larger_table.getNumberOfRows();
	for (size_t i = 0; i < number_of_rows; i++) {
		ResultRow row;
		for (string const& common_syn : common_synonyms) {
			row.insert({common_syn, larger_table.getColumn(common_syn)[i]});
		}

		ResultTable sub_table = smaller_table.getSubTableWithRow(row);
		size_t sub_table_size = sub_table.getNumberOfRows();
		if (sub_table_size > 0) {
			// Common synonyms
			for (string const& common_syn : common_synonyms) {
				vector<string>& col = new_table[common_syn];
				col.insert(col.end(), sub_table_size, row[common_syn]);
			}

			// Old synonyms
			for (string const& old_syn : old_synonyms) {
				vector<string>& col = new_table[old_syn];
				col.insert(col.end(), sub_table_size, larger_table.getColumn(old_syn).at(i));
			}

			// New synonyms
			for (string const& new_syn : new_synonyms) {
				vector<string> sub_table_col = sub_table.getColumn(new_syn);
				vector<string>& col = new_table[new_syn];
				col.insert(col.end(), sub_table_col.begin(), sub_table_col.end());
			}
		}
	}

	larger_set.insert(new_synonyms.begin(), new_synonyms.end());
	return ResultTable(larger_set, new_table);
}

QP::Types::ResultTable QP::Types::ResultTable::getSubTableWithRow(const ResultRow& row) {
	if (row.empty()) {
		return *this;
	}

	ResultTable sub_table = ResultTable(synonyms_stored, table);
	size_t number_of_rows = getNumberOfRows();

	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		if (!sub_table.isRowMatch(row, pos)) {
			sub_table.removeRow(pos);
		} else {
			pos++;
		}
	}

	return sub_table;
}

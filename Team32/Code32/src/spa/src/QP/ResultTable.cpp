#include "ResultTable.h"

#include <iostream>

QP::Types::ResultTable::ResultTable() {}

QP::Types::ResultTable::ResultTable(unordered_set<string> synonyms_stored, unordered_map<string, vector<string>> table)
	: synonyms_stored(synonyms_stored), table(table) {}

size_t QP::Types::ResultTable::getNumberOfRows() {
	if (table.empty()) {
		return 0;
	}

	return table.begin()->second.size();
}

size_t QP::Types::ResultTable::getNumberOfColumns() { return synonyms_stored.size(); }

unordered_map<string, vector<string>> QP::Types::ResultTable::getTable() { return table; }

unordered_set<string> QP::Types::ResultTable::getSynonymsStored() { return synonyms_stored; }

ResultColumn QP::Types::ResultTable::getColumn(const string& synonym) { return table.at(synonym); }

bool QP::Types::ResultTable::contains(const ResultRow& row) {
	size_t number_of_rows = getNumberOfRows();
	for (size_t i = 0; i < number_of_rows; i++) {
		if (isRowMatch(row, i)) {
			return true;
		}
	}

	return false;
}

bool QP::Types::ResultTable::isRowMatch(const ResultRow& row, size_t row_number) {
	bool has_match = true;
	for (const auto& iterator : row) {
		if (table.at(iterator.first)[row_number] != iterator.second) {
			has_match = false;
			break;
		}
	}

	return has_match;
}

void QP::Types::ResultTable::removeRow(size_t row_number) {
	for (auto iterator = table.begin(); iterator != table.end(); ++iterator) {
		vector<string> col = iterator->second;
		// Narrowing conversion, implementation defined behaviour for values greater than long.
		col.erase(col.begin() + static_cast<vector<string>::difference_type>(row_number));
		table[iterator->first] = col;
	}
}

void QP::Types::ResultTable::removeDuplicateRows() {
	unordered_set<string> rows;
	size_t number_of_rows = getNumberOfRows();

	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		string row;
		for (auto const& pair : table) {
			row.append(pair.second[pos]);
			row.append(" ");
		}

		if (rows.find(row) != rows.end()) {
			removeRow(pos);
		} else {
			pos++;
			rows.insert(row);
		}
	}
}

void QP::Types::ResultTable::insertColumn(const string& synonym, const ResultColumn& column) {
	synonyms_stored.insert(synonym);
	table.insert({synonym, column});
}

void QP::Types::ResultTable::filterBySelect(const QP::Types::DeclarationList& select_list) {
	unordered_map<string, vector<string>> filtered_table;
	unordered_set<string> filtered_synonyms;

	for (auto const& declaration : select_list) {
		filtered_table.insert({declaration.symbol, this->table[declaration.symbol]});
		filtered_synonyms.insert(declaration.symbol);
	}

	this->table = filtered_table;
	this->synonyms_stored = filtered_synonyms;
	removeDuplicateRows();
}

QP::Types::ResultTable QP::Types::ResultTable::joinTables(pair<ResultTable&, ResultTable&> tables) {
	ResultTable larger_table = tables.first;
	ResultTable smaller_table = tables.second;

	if (tables.first.getNumberOfColumns() >= tables.second.getNumberOfColumns()) {
		larger_table = tables.first;
		smaller_table = tables.second;
	} else {
		larger_table = tables.second;
		smaller_table = tables.first;
	}

	unordered_set<string> larger_set = larger_table.getSynonymsStored();
	for (string const& synonym : smaller_table.getSynonymsStored()) {
		if (larger_set.find(synonym) == larger_set.end()) {
			return joinWithDifferentSynonym(larger_table, smaller_table);
		}
	}

	return joinWithSameSynonym(larger_table, smaller_table);
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

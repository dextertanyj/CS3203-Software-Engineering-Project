#include "ResultTable.h"

QP::Types::ResultTable::ResultTable() {}

QP::Types::ResultTable::ResultTable(unordered_map<string, vector<string>> table) : table(table) {}

QP::Types::ResultTable::ResultTable() {}

size_t QP::Types::ResultTable::getTableSize() {
	if (table.empty()) {
		return 0;
	}

	return table.begin()->second.size();
}

ResultColumn QP::Types::ResultTable::getColumn(const string& synonym) {
	return table.at(synonym);
}

bool QP::Types::ResultTable::contains(const ResultRow& row) {
	size_t number_of_rows = getTableSize();
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

void QP::Types::ResultTable::removeDuplicateRows() {}

void QP::Types::ResultTable::insertColumn(const string& synonym, const ResultColumn& column) {}

QP::Types::ResultTable QP::Types::ResultTable::getSubTableWithRow(const ResultRow& row) {
	if (row.empty()) {
		return table;
	}

	ResultTable sub_table = ResultTable(table);
	size_t number_of_rows = getTableSize();

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

#include "QP/QueryResult.h"

QP::QueryResult::QueryResult() { this->result = false; }

QP::QueryResult::QueryResult(bool result) { this->result = result; }

bool QP::QueryResult::getResult() const { return result; }

unordered_map<string, vector<string>> QP::QueryResult::getTable() { return table; }

unordered_set<string> QP::QueryResult::getSynonymsStored() { return synonyms_stored; }

vector<string> QP::QueryResult::getSynonymResult(const string& synonym) { return table.at(synonym); }

void QP::QueryResult::addColumn(const string& synonym, const vector<string>& column) {
	if (column.empty()) {
		result = false;
		return;
	}

	result = true;
	synonyms_stored.insert(synonym);
	table.insert({synonym, column});
}

void QP::QueryResult::joinResult(QueryResult& query_result) {
	for (string const& synonym : query_result.getSynonymsStored()) {
		if (this->synonyms_stored.find(synonym) == synonyms_stored.end()) {
			joinWithDifferentSynonym(query_result);
			return;
		}
	}

	joinWithSameSynonym(query_result);
}

void QP::QueryResult::joinWithDifferentSynonym(QueryResult& query_result) {
	unordered_set<string> common_synonyms;
	unordered_set<string> old_synonyms;
	unordered_set<string> new_synonyms;
	unordered_map<string, vector<string>> new_table;

	for (string const& synonym : query_result.getSynonymsStored()) {
		new_table.insert({synonym, {}});
		if (this->synonyms_stored.find(synonym) != synonyms_stored.end()) {
			common_synonyms.insert(synonym);
		} else {
			new_synonyms.insert(synonym);
		}
	}

	for (string const& old_synonym : this->getSynonymsStored()) {
		if (common_synonyms.find(old_synonym) == common_synonyms.end()) {
			new_table.insert({old_synonym, {}});
			old_synonyms.insert(old_synonym);
		}
	}

	size_t number_of_rows = table.begin()->second.size();
	for (size_t i = 0; i < number_of_rows; i++) {
		unordered_map<string, string> row;
		for (string const& common_syn : common_synonyms) {
			row.insert({common_syn, table[common_syn][i]});
		}

		unordered_map<string, vector<string>> sub_table = query_result.getSubTableWithRow(row);
		if (!sub_table.begin()->second.empty()) {
			size_t sub_table_size = sub_table.begin()->second.size();

			// Common synonyms
			for (string const& common_syn : common_synonyms) {
				vector<string>& col = new_table[common_syn];
				col.insert(col.end(), sub_table_size, row[common_syn]);
			}

			// Old synonyms
			for (string const& old_syn : old_synonyms) {
				vector<string>& col = new_table[old_syn];
				col.insert(col.end(), sub_table_size, table[old_syn].at(i));
			}

			// New synonyms
			for (string const& new_syn : new_synonyms) {
				vector<string> sub_table_col = sub_table[new_syn];
				vector<string>& col = new_table[new_syn];
				col.insert(col.end(), sub_table_col.begin(), sub_table_col.end());
			}
		}
	}

	this->table = new_table;
	this->synonyms_stored.insert(new_synonyms.begin(), new_synonyms.end());
	if (new_table.begin()->second.empty()) {
		this->result = false;
	}
}

void QP::QueryResult::joinWithSameSynonym(QueryResult& query_result) {
	unordered_map<string, vector<string>> final_result;
	unordered_map<string, vector<string>> smaller_table;
	unordered_set<string> synonyms;
	if (this->table.size() > query_result.getTable().size()) {
		final_result = this->table;
		smaller_table = query_result.getTable();
		synonyms = query_result.getSynonymsStored();
	} else {
		final_result = query_result.getTable();
		smaller_table = this->table;
		synonyms = this->synonyms_stored;
	}

	size_t number_of_rows = final_result.begin()->second.size();
	unordered_map<string, vector<string>> temp = final_result;
	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		unordered_map<string, string> row;
		for (const string& synonym : synonyms) {
			row.insert({synonym, temp.at(synonym)[i]});
		}

		if (!contains(smaller_table, row)) {
			removeRow(final_result, pos);
		} else {
			pos++;
		}
	}

	this->synonyms_stored = synonyms;
	this->table = final_result;
	if (final_result.begin()->second.empty()) {
		this->result = false;
	}
}

bool QP::QueryResult::contains(unordered_map<string, vector<string>>& table, const unordered_map<string, string>& row) {
	size_t number_of_rows = table.begin()->second.size();
	for (size_t i = 0; i < number_of_rows; i++) {
		if (isRowMatch(row, table, i)) {
			return true;
		}
	}

	return false;
}

bool QP::QueryResult::isRowMatch(const unordered_map<string, string>& row, unordered_map<string, vector<string>>& table, size_t row_number) {
	bool has_match = true;
	for (const auto& iterator : row) {
		if (table.at(iterator.first)[row_number] != iterator.second) {
			has_match = false;
			break;
		}
	}

	return has_match;
}

void QP::QueryResult::removeRow(unordered_map<string, vector<string>>& table, size_t row_number) {
	for (auto iterator = table.begin(); iterator != table.end(); ++iterator) {
		vector<string> col = iterator->second;
		col.erase(col.begin() + row_number);
		table[iterator->first] = col;
	}
}

unordered_map<string, vector<string>> QP::QueryResult::getSubTableWithRow(const unordered_map<string, string>& row) {
	if (row.empty()) {
		return table;
	}

	unordered_map<string, vector<string>> sub_table = this->table;
	size_t number_of_rows = table.begin()->second.size();

	size_t pos = 0;
	for (size_t i = 0; i < number_of_rows; i++) {
		if (!isRowMatch(row, sub_table, pos)) {
			removeRow(sub_table, pos);
		} else {
			pos++;
		}
	}

	return sub_table;
}

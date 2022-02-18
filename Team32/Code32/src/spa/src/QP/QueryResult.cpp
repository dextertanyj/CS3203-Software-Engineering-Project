#include "QP/QueryResult.h"

QueryResult::QueryResult() {
	this->result = false;
}

QueryResult::QueryResult(bool result) {
	this->result = result;
}

bool QueryResult::getResult() const { return result; }

unordered_map<string, vector<string>> QueryResult::getTable() {
	return table;
}

unordered_set<string> QueryResult::getSynonymsStored() {
	return synonyms_stored;
}

vector<string> QueryResult::getSynonymResult(const string& synonym) { return table.at(synonym); }

void QueryResult::addColumn(const string& synonym, const vector<string>& column) {
	if (column.empty()) {
		result = false;
		return;
	}

	result = true;
	synonyms_stored.insert(synonym);
	table.insert({ synonym, column });
}

void QueryResult::joinResult(QueryResult& query_result) {
	// Evaluator will ensure that queryResult has less columns when joining.
	// Check whether all synonyms in the smaller table is in the larger table.
	for (string const& synonym : query_result.getSynonymsStored()) {
		if (this->synonyms_stored.find(synonym) == synonyms_stored.end()) {
			joinWithDifferentSynonym(query_result);
			return;
		}
	}

	joinWithSameSynonym(query_result);
}

void QueryResult::joinWithDifferentSynonym(QueryResult& query_result) {
	// For iteration one, there will only be one common synonym between the two tables.
	// The evaluator will join the table without the selected synonym to the one containing it.
	// For now, we only check whether the common synonym is contained in both tables instead of merging
	// them into a bigger table.
	string common_synonym;
	for (string const& synonym : query_result.getSynonymsStored()) {
		if (this->synonyms_stored.find(synonym) != synonyms_stored.end()) {
			common_synonym = synonym;
			break;
		}
	}

	unordered_map<string, vector<string>> final_result = this->table;
	int number_of_rows = table.begin()->second.size();
	int pos = 0;

	for (int i = 0; i < number_of_rows; i++) {
		if (!query_result.contains({{common_synonym, this->table[common_synonym][i]}})) {
			removeRow(final_result, pos);
		} else {
			pos++;
		}
	}

	this->table = final_result;
	if (final_result.begin()->second.empty()) {
		this->result = false;
	}
}

void QueryResult::joinWithSameSynonym(QueryResult& query_result) {
	int number_of_rows = table.begin()->second.size();
	unordered_map<string, vector<string>> final_result = this->table;
	int pos = 0;

	for (int i = 0; i < number_of_rows; i++) {
		unordered_map<string, string> row;
		for (const string& synonym : query_result.getSynonymsStored()) {
			row.insert({ synonym, table.at(synonym)[i] });
		}

		if (!query_result.contains(row)) {
			removeRow(final_result, pos);
		} else {
			pos++;
		}
	}

	this->table = final_result;
	if (final_result.begin()->second.empty()) {
		this->result = false;
	}
}

bool QueryResult::contains(unordered_map<string, string> row) {
	int number_of_rows = table.begin()->second.size();
	for (int i = 0; i < number_of_rows; i++) {
		bool has_match = true;
		for (auto& iterator : row) {
			if (table.at(iterator.first)[i] != iterator.second) {
				has_match = false;
				break;
			}
		}

		if (has_match) {
			return true;
		}
	}

	return false;
}

void QueryResult::removeRow(unordered_map<string, vector<string>>& table, int row_number) {
	for (auto iterator = table.begin(); iterator != table.end(); ++iterator) {
		vector<string> col = iterator->second;
		col.erase(col.begin() + row_number);
		table[iterator->first] = col;
	}
}
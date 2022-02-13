#include "QP/QueryResult.h"

QueryResult::QueryResult() {
	this->result = false;
}

bool QueryResult::getResult() {
	return result;
}

unordered_map<string, vector<string>> QueryResult::getTable() {
	return table;
}

unordered_set<string> QueryResult::getSynonymsStored() {
	return synonymsStored;
}

vector<string> QueryResult::getSynonymResult(string synonym) {
	return table.at(synonym);
}

void QueryResult::addColumn(string synonym, vector<string> column) {
	if (column.size() == 0) {
		result = false;
		return;
	}

	result = true;
	synonymsStored.insert(synonym);
	table.insert({ synonym, column });
}

void QueryResult::joinResult(QueryResult& queryResult) {
	// Evaluator will ensure that queryResult has less columns when joining.
	// Check whether all synonyms in the smaller table is in the larger table.
	for (string const &synonym : queryResult.getSynonymsStored()) {
		if (this->synonymsStored.find(synonym) == synonymsStored.end()) {
			joinWithDifferentSynonym(queryResult);
			return;
		}
	}

	joinWithSameSynonym(queryResult);
}

void QueryResult::joinWithDifferentSynonym(QueryResult& queryResult) {
	// For iteration one, there will only be one common synonym between the two tables.
	// The evaluator will join the table without the selected synonym to the one containing it.
	// For now, we only check whether the common synonym is contained in both tables instead of merging
	// them into a bigger table.
	string commonSynonym;
	for (string const& synonym : queryResult.getSynonymsStored()) {
		if (this->synonymsStored.find(synonym) != synonymsStored.end()) {
			commonSynonym = synonym;
			break;
		}
	}
	
	unordered_map<string, vector<string>> finalResult = this->table;
	int numberOfRows = table.begin()->second.size();
	int pos = 0;

	for (int i = 0; i < numberOfRows; i++) {
		if (!queryResult.contains({ { commonSynonym, this->table[commonSynonym][i] } })) {
			removeRow(finalResult, pos);
		} else {
			pos++;
		}
	}

	this->table = finalResult;
}

void QueryResult::joinWithSameSynonym(QueryResult& queryResult) {
	int numberOfRows = table.begin()->second.size();
	unordered_map<string, vector<string>> finalResult = this->table;
	int pos = 0;

	for (int i = 0; i < numberOfRows; i++) {
		unordered_map<string, string> row;
		for (string synonym : queryResult.getSynonymsStored()) {
			row.insert({ synonym, table.at(synonym)[i] });
		}

		if (!queryResult.contains(row)) {
			removeRow(finalResult, pos);
		} else {
			pos++;
		}
	}

	this->table = finalResult;
}

bool QueryResult::contains(unordered_map<string, string> row) {
	int numberOfRows = table.begin()->second.size();
	for (int i = 0; i < numberOfRows; i++) {
		bool hasMatch = true;
		for (auto iterator = row.begin(); iterator != row.end(); ++iterator) {
			if (table.at(iterator->first)[i] != iterator->second) {
				hasMatch = false;
				break;
			}
		}

		if (hasMatch) {
			return true;
		}
	}
	
	return false;
}

void QueryResult::removeRow(unordered_map<string, vector<string>> &table, int rowNumber) {
	for (auto iterator = table.begin(); iterator != table.end(); ++iterator) {
		vector<string> col = iterator->second;
		col.erase(col.begin() + rowNumber);
		table[iterator->first] = col;
	}
}
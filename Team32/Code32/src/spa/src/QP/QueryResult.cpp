#include "QP/QueryResult.h"
#include <iostream>
#include <queue>
#include <set>

QP::QueryResult::QueryResult() : result(false) {}

QP::QueryResult::QueryResult(bool result) : result(result) {}

QP::QueryResult::QueryResult(vector<string> synonyms) : result(false), table(ResultTable(move(synonyms))) {}

QP::QueryResult::QueryResult(ResultTable result_table) : result(true), table(move(result_table)) {}

bool QP::QueryResult::getResult() const { return result; }

ResultTable QP::QueryResult::getTable() { return table; }

ResultColumn QP::QueryResult::getSynonymResult(const string& synonym) { return table.getColumn(synonym); }

ResultRow QP::QueryResult::getRowWithOrder(const vector<string>& synonyms, size_t row_number) {
	return table.getRowWithOrder(synonyms, row_number);
}

size_t QP::QueryResult::getNumberOfRows() { return table.getNumberOfRows(); }

void QP::QueryResult::addRow(const ResultRow& row) {
	result = true;
	table.insertRow(row);
}

void QP::QueryResult::filterBySelect(const QP::Types::DeclarationList& select_list) {
	if (select_list.empty()) {
		table = ResultTable();
		return;
	}

	table = table.filterBySelect(select_list);
}

QP::QueryResult QP::QueryResult::joinResults(vector<QueryResult>& results) {
	if (results.empty()) {
		return {};
	}

	ResultTable table = results[0].getTable();
	for (size_t i = 1; i < results.size(); i++) {
		table = ResultTable::joinTables(table, results[i].getTable());
		if (table.getNumberOfRows() == 0) {
			return {};
		}
	}

	return QueryResult(table);
}

QP::QueryResult QP::QueryResult::joinIntraGroupResults(vector<QueryResult>& results) {
	if (results.empty()) {
		return {};
	}

	sort(results.begin(), results.end(), compareLength);

	// create a mapping from synonym => index of results with that synonym
	unordered_map<string, vector<size_t>> synonym_to_index_map = getSynonymIndexMap(results);
	set<size_t> completed;

	ResultTable table = results[0].getTable();
	priority_queue<size_t, vector<size_t>, greater<>> queue;
	for (auto & syn :results[0].getTable().getSynonymsStored()) {
		if (synonym_to_index_map.find(syn) != synonym_to_index_map.end()) {
			vector<size_t> indexes = synonym_to_index_map.find(syn)->second;
			for (auto& ind : indexes) {
				if (completed.find(ind) == completed.end()) {
					queue.push(ind);
					completed.insert(ind);
				}
			}
			synonym_to_index_map.erase(syn);
		}
	}
	while (!queue.empty()) {
		ResultTable toAdd = results[queue.top()].getTable();
		table = ResultTable::joinTables(table, toAdd);
		queue.pop();

		if (table.getNumberOfRows() == 0) {
			return {};
		}
		for (auto & syn :toAdd.getSynonymsStored()) {
			if (synonym_to_index_map.find(syn) != synonym_to_index_map.end()) {
				vector<size_t> indexes = synonym_to_index_map.find(syn)->second;
				for (auto& ind : indexes) {
					if (completed.find(ind) == completed.end()) {
						queue.push(ind);
						completed.insert(ind);
					}
				}
				synonym_to_index_map.erase(syn);
			}
		}
	}
	return QueryResult(table);
}

bool QP::QueryResult::compareLength(QP::QueryResult result1, QP::QueryResult result2) {
	return result1.getNumberOfRows() < result2.getNumberOfRows();
}

unordered_map<string, vector<size_t>> QP::QueryResult::getSynonymIndexMap(vector<QueryResult>& results) {
	unordered_map<string, vector<size_t>> synonym_to_index_map;
	for (size_t i = 1; i < results.size(); i++) {
		ResultTable table = results[i].getTable();
		for (auto & synonym : table.getSynonymsStored()) {
			if (synonym_to_index_map.find(synonym) == synonym_to_index_map.end()) {
				synonym_to_index_map[synonym] = { i };
			} else {
				synonym_to_index_map.find(synonym)->second.push_back(i);
			}
		}
	}
	return synonym_to_index_map;
}

#include "QP/QueryResult.h"

#include <algorithm>
#include <utility>

using namespace std;

QP::QueryResult::QueryResult() : result(false) {}

QP::QueryResult::QueryResult(bool result) : result(result) {}

QP::QueryResult::QueryResult(vector<string> synonyms) : result(false), table(ResultTable(move(synonyms))) {}

QP::QueryResult::QueryResult(ResultTable result_table) : result(true), table(move(result_table)) {}

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

bool QP::QueryResult::getResult() const { return result; }

size_t QP::QueryResult::getNumberOfRows() const { return table.getNumberOfRows(); }

ResultTable QP::QueryResult::getTable() const { return table; }

bool QP::QueryResult::containsSynonym(const string& synonym) const {
	auto synonyms = table.getSynonymsStoredMap();
	return synonyms.find(synonym) != synonyms.end();
}

vector<string> QP::QueryResult::getSynonymsStored() const { return table.getSynonymsStored(); }

ResultColumn QP::QueryResult::getSynonymResult(const string& synonym) const { return table.getColumn(synonym); }

ResultRow QP::QueryResult::getRowWithOrder(const vector<string>& synonyms, size_t row_number) const {
	return table.getRowWithOrder(synonyms, row_number);
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

QP::QueryResult QP::QueryResult::joinIntraGroupResults(vector<QP::QueryResult>& results) {
	if (results.empty()) {
		return {};
	}

	// Order of join operations do not matter for less than 3 results.
	if (results.size() < 3) {
		return joinResults(results);
	}
	sort(results.begin(), results.end(), compareLength);

	unordered_set<size_t> completed = {0};
	ResultTable table = results[0].getTable();

	unordered_map<string, vector<size_t>> synonym_to_index_map = getSynonymToResultIndexMap(results);
	priority_queue<size_t, vector<size_t>, greater<>> queue;
	findNeighbours(results[0], synonym_to_index_map, queue, completed);

	while (!queue.empty()) {
		QP::QueryResult curr = results[queue.top()];
		queue.pop();
		ResultTable to_add = curr.getTable();
		table = ResultTable::joinTables(table, to_add);
		if (table.getNumberOfRows() == 0) {
			return {};
		}
		// All results are either in the queue or have been joined into the current result.
		if (completed.size() == results.size()) {
			continue;
		}
		findNeighbours(curr, synonym_to_index_map, queue, completed);
	}

	return QueryResult(table);
}

bool QP::QueryResult::compareLength(const QP::QueryResult& lhs, const QP::QueryResult& rhs) {
	return lhs.getNumberOfRows() < rhs.getNumberOfRows();
}

unordered_map<string, vector<size_t>> QP::QueryResult::getSynonymToResultIndexMap(const vector<QP::QueryResult>& results) {
	unordered_map<string, vector<size_t>> synonym_to_index_map;
	// Search begins with table 0 already in the queue, so it can be ignored.
	for (size_t i = 1; i < results.size(); i++) {
		for (const auto& synonym : results[i].getSynonymsStored()) {
			synonym_to_index_map[synonym].emplace_back(i);
		}
	}
	return synonym_to_index_map;
}

void QP::QueryResult::findNeighbours(const QP::QueryResult& current, unordered_map<string, vector<size_t>>& synonym_to_index_map,
                                     priority_queue<size_t, vector<size_t>, greater<>>& queue, unordered_set<size_t>& completed) {
	for (const auto& syn : current.getSynonymsStored()) {
		auto iter = synonym_to_index_map.find(syn);
		if (iter == synonym_to_index_map.end()) {
			continue;
		}
		vector<size_t> indexes = iter->second;
		addNeighboursToQueue(indexes, queue, completed);
		synonym_to_index_map.erase(syn);
	}
}

void QP::QueryResult::addNeighboursToQueue(const vector<size_t>& indexes, priority_queue<size_t, vector<size_t>, greater<>>& queue,
                                           unordered_set<size_t>& completed) {
	for (const auto& idx : indexes) {
		if (completed.find(idx) != completed.end()) {
			continue;
		}
		queue.push(idx);
		completed.emplace(idx);
	}
}

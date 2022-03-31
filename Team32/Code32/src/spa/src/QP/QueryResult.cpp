#include "QP/QueryResult.h"

#include <queue>

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

size_t QP::QueryResult::getNumberOfRows() const { return table.getNumberOfRows(); }

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
	if (results.size() < 3) {
		return joinResults(results);
	}
	sort(results.begin(), results.end(), compareLength);

	unordered_map<string, vector<size_t>> synonym_to_index_map = getSynonymIndexMap(results);
	unordered_set<size_t> completed = {0};
	ResultTable table = results[0].getTable();
	priority_queue<size_t, vector<size_t>, greater<>> queue;
	findNeighbours(results[0], synonym_to_index_map, queue, completed);
	while (!queue.empty()) {
		QueryResult curr = results.at(queue.top());
		queue.pop();
		ResultTable to_add = curr.getTable();
		table = ResultTable::joinTables(table, to_add);
		if (table.getNumberOfRows() == 0) {
			return {};
		}
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

unordered_map<string, vector<size_t>> QP::QueryResult::getSynonymIndexMap(const vector<QueryResult>& results) {
	unordered_map<string, vector<size_t>> synonym_to_index_map;
	for (size_t i = 1; i < results.size(); i++) {
		for (const auto& synonym : results[i].getSynonymsStored()) {
			if (synonym_to_index_map.find(synonym) == synonym_to_index_map.end()) {
				synonym_to_index_map.insert({synonym, {i}});
			} else {
				synonym_to_index_map.find(synonym)->second.push_back(i);
			}
		}
	}
	return synonym_to_index_map;
}

void QP::QueryResult::findNeighbours(const QP::QueryResult& current, unordered_map<string, vector<size_t>>& synonym_to_index_map,
                                     priority_queue<size_t, vector<size_t>, greater<>>& queue, unordered_set<size_t>& completed) {
	for (const auto& syn : current.getSynonymsStored()) {
		if (synonym_to_index_map.find(syn) != synonym_to_index_map.end()) {
			vector<size_t> indexes = synonym_to_index_map.find(syn)->second;
			addNeighboursToQueue(indexes, queue, completed);
			synonym_to_index_map.erase(syn);
		}
	}
}

void QP::QueryResult::addNeighboursToQueue(const vector<size_t>& indexes, priority_queue<size_t, vector<size_t>, greater<>>& queue,
                                           unordered_set<size_t>& completed) {
	for (const auto& idx : indexes) {
		if (completed.find(idx) != completed.end()) {
			continue;
		}
		queue.push(idx);
		completed.insert(idx);
	}
}

vector<string> QP::QueryResult::getSynonymsStored() const { return table.getSynonymsStored(); }

#include "QP/QueryResult.h"

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

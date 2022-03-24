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

void QP::QueryResult::filterBySelect(const QP::Types::DeclarationList& select_list) { table = table.filterBySelect(select_list); }

QP::QueryResult QP::QueryResult::joinResult(QueryResult result_one, QueryResult result_two) {
	if (!result_one.getResult() || !result_two.getResult()) {
		return {};
	}

	ResultTable table = ResultTable::joinTables(result_one.getTable(), result_two.getTable());

	if (table.getNumberOfRows() == 0) {
		return {};
	}

	return QueryResult(table);
}

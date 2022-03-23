#include "QP/QueryResult.h"

QP::QueryResult::QueryResult() : result(false) {}

QP::QueryResult::QueryResult(bool result) : result(result) {}

QP::QueryResult::QueryResult(vector<string> synonyms) : result(false), table(ResultTable(synonyms)) {}

bool QP::QueryResult::getResult() const { return result; }

ResultTable QP::QueryResult::getTable() { return table; }

ResultColumn QP::QueryResult::getSynonymResult(const string& synonym) { return table.getColumn(synonym); }

size_t QP::QueryResult::getNumberOfRows() { return table.getNumberOfRows(); }

void QP::QueryResult::addRow(const ResultRow& row) {
	result = true;
	table.insertRow(row);
}

void QP::QueryResult::joinResult(QueryResult& query_result) {
	this->table = ResultTable::joinTables(this->table, query_result.getTable());
	this->result = table.getNumberOfRows() == 0 ? false : true;
}

void QP::QueryResult::filterBySelect(const QP::Types::DeclarationList& select_list) { table = table.filterBySelect(select_list); }

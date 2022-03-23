#include "QP/QueryResult.h"

QP::QueryResult::QueryResult() { this->result = false; }

QP::QueryResult::QueryResult(bool result) { this->result = result; }

bool QP::QueryResult::getResult() const { return result; }

ResultTable QP::QueryResult::getTable() { return table; }

ResultColumn QP::QueryResult::getSynonymResult(const string& synonym) { return table.getColumn(synonym); }

size_t QP::QueryResult::getNumberOfRows() { return table.getNumberOfRows(); }

void QP::QueryResult::addColumn(const string& synonym, const ResultColumn& column) {
	if (column.empty()) {
		result = false;
		return;
	}

	result = true;
	table.insertColumn(synonym, column);
}

void QP::QueryResult::joinResult(QueryResult& query_result) {
	this->table = ResultTable::joinTables(make_pair(this->table, query_result.getTable()));
	this->result = table.getNumberOfRows() == 0 ? false : true;
}

void QP::QueryResult::filterBySelect(const QP::Types::DeclarationList& select_list) { table = table.filterBySelect(select_list); }

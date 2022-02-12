#include "QP/QueryResult.h"

QueryResult::QueryResult() {
	this->result = false;
}

bool QueryResult::getResult() {
	return result;
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
	table.insert({ synonym, column });
}

void QueryResult::joinResult(QueryResult& queryResult) {
	// TODO(ypinhsuan)
}

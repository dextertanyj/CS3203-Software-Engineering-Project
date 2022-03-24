#include "QP/QueryFormatter.h"

#include <algorithm>
#include <string>
#include <vector>

#include "QP/Types.h"

#define TUPLE_SEPERATOR " "
#define TRUE "TRUE"
#define FALSE "FALSE"

using namespace std;
using QP::Types::Declaration;
using QP::Types::DeclarationList;

vector<string> QP::QueryFormatter::formatResult(QueryProperties& query_properties, QueryResult& query_result) {
	DeclarationList select_list = query_properties.getSelectList();
	if (select_list.empty()) {
		return formatBooleanResult(query_result);
	}

	return formatNonBooleanResult(query_properties, query_result);
}

vector<string> QP::QueryFormatter::formatBooleanResult(QueryResult& query_result) {
	string result = query_result.getResult() ? TRUE : FALSE;
	return {result};
}

vector<string> QP::QueryFormatter::formatNonBooleanResult(QueryProperties& query_properties, QueryResult& query_result) {
	if (!query_result.getResult()) {
		return {};
	}

	size_t table_size = query_result.getNumberOfRows();
	vector<string> result(table_size);
	DeclarationList select_list = query_properties.getSelectList();
	vector<string> synonyms(select_list.size());
	transform(select_list.begin(), select_list.end(), synonyms.begin(), [](Declaration& declaration) { return declaration.symbol; });

	for (size_t i = 0; i < table_size; i++) {
		ResultRow row = query_result.getRowWithOrder(synonyms, i);
		string row_string;

		for (string const& value : row) {
			row_string.append(value);
			row_string.append(TUPLE_SEPERATOR);
		}
		row_string.pop_back();
		result[i] = row_string;
	}

	return result;
}

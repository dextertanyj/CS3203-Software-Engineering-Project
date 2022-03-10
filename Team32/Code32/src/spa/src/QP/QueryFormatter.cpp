#include "QP/QueryFormatter.h"

#include <string>

#include "QP/QueryTypes.h"

using namespace std;

vector<string> QP::QueryFormatter::formatResult(QueryProperties& query_properties, QueryResult& query_result) {
	DeclarationList select_list = query_properties.getSelectList();
	if (select_list.empty()) {
		return formatBooleanResult(query_result);
	}

	if (!query_result.getResult()) {
		return {};
	}

	vector<string> result = query_result.getSynonymResult(select_list[0].symbol);
	// Remove duplicates from result
	unordered_set<string> set;
	for (const string& value : result) {
		set.insert(value);
	}

	result.assign(set.begin(), set.end());

	return result;
}

vector<string> QP::QueryFormatter::formatBooleanResult(QueryResult& query_result) {
	string result = query_result.getResult() ? "TRUE" : "FALSE";
	return {result};
}

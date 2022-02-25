#include "QP/QueryFormatter.h"

#include <string>

#include "QP/QueryTypes.h"

using namespace std;

vector<string> QP::QueryFormatter::formatResult(QueryProperties& query_properties, QueryResult& query_result) {
	Declaration select = query_properties.getSelect();

	if (!query_result.getResult()) {
		return {};
	}

	vector<string> result = query_result.getSynonymResult(select.symbol);
	// Remove duplicates from result
	unordered_set<string> set;
	for (const string& value : result) {
		set.insert(value);
	}

	result.assign(set.begin(), set.end());

	return result;
}

#include "QP/QueryFormatter.h"

#include <string>

#include "QP/QueryTypeDefs.h"

using namespace std;

vector<string> QueryFormatter::formatResult(QueryProperties& queryProperties, QueryResult& queryResult) {
	Declaration select = queryProperties.getSelect();

	if (!queryResult.getResult()) {
		return {};
	}

	vector<string> result = queryResult.getSynonymResult(select.symbol);
	// Remove duplicates from result
	unordered_set<string> set;
	for (string value : result) {
		set.insert(value);
	}

	result.assign(set.begin(), set.end());

	return result;
}

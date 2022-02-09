#include "QP/QueryFormatter.h"

#include <sstream>
#include <string>

#include "QP/QueryTypeDefs.h"

using namespace std;

vector<string> QueryFormatter::formatResult(QueryProperties& queryProperties, QueryResult& queryResult) {
	Declaration select = queryProperties.getSelect();
	vector<string> result;

	if (select.type == DesignEntity::stmt) {
		StmtRefList stmt_list = queryResult.getStmtRefList();
		for (auto iter = stmt_list.begin(); iter < stmt_list.end(); ++iter) {
			result.push_back(to_string(*iter));
		}
	}

	return result;
}

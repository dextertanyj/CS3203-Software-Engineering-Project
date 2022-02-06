#include "QP/QueryFormatter.h"

#include <sstream>
#include <string>

#include "QP/QueryTypeDefs.h"

using std::string;
using std::stringstream;
using std::vector;

string QueryFormatter::formatResult(QueryProperties& queryProperties, QueryResult& queryResult) {
	Declaration select = queryProperties.getSelect();
	
	if (select.type == DesignEntity::stmt) {
		StmtRefList stmtList = queryResult.getStmtRefList();
		return stmtListToString(stmtList);
	}

	return "invalid";
}

string QueryFormatter::stmtListToString(StmtRefList& stmts) {
	if (stmts.empty()) {
		return "none";
	}

	std::stringstream result;
	for (size_t i = 0; i < stmts.size(); ++i) {
		if (i != 0) {
			result << ", ";
		}
		result << stmts[i];
	}
	
	return result.str();
}

#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "QueryEvaluator.h"
#include "QueryProperties.h"
#include "QueryResult.h"

using std::string;
using std::vector;
using std::runtime_error;

struct QuerySyntaxException : public runtime_error {
    using runtime_error::runtime_error;
};

class QueryProcessor {
public:
	string processQuery(string query);

private:
	void tokenizeQuery(string query);
	void parseQuery();
	int parseDeclaration(int tokenIndex);
	int parseSuchThat(int tokenIndex);
	int parsePattern(int tokenIndex);
	string formatResult();

	vector<string> queryTokens;
	QueryProperties queryProperties;
	QueryResult queryResult;
};

#endif // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_
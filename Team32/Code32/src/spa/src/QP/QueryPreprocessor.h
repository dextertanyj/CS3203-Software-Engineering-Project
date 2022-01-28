#ifndef QUERY_PREPROCESSOR_H_
#define QUERY_PREPROCESSOR_H_

#include <string>

#include "QueryProperties.h"

using namespace std;

struct QuerySyntaxException : public std::runtime_error {
    using runtime_error::runtime_error;
};

class QueryPreprocessor {
public:
	static string processQuery(string query);

private:
	static vector<string> tokenizeQuery(string query);
	static QueryProperties parseQuery(vector<string> queryTokens);
};

#endif // QUERY_PREPROCESSOR_H_
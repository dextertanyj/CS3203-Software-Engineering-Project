#include "QueryPreprocessor.h"
#include "QueryProperties.h"
#include <regex>
#include <string>

using namespace std;

static regex invalidCharsRegex = regex("([^a-zA-Z0-9 ,\\(\\);\\+\\-\\*\\/%\\n])");
static regex queryTokenRegex = regex("((Follows|Parents)\\*|such that|[a-zA-Z][a-zA-Z0-9]*|[0-9]+|\\(|\\)|;|\\+|-|\\*|\\/|%|_|,|\")");

string QueryPreprocessor::processQuery(string query) {
	return NULL;
}

vector<string> QueryPreprocessor::tokenizeQuery(string query) {

	if (regex_search(query, invalidCharsRegex)) {
		throw QuerySyntaxException("Query included invalid characters");
	}
	vector<string> queryTokens;
	auto words_begin = sregex_iterator(
		query.begin(), 
		query.end(), 
		queryTokenRegex
	);
	auto words_end = sregex_iterator();
	for (sregex_iterator i = words_begin; i != words_end; ++i) {
		smatch match = *i;
		queryTokens.push_back(match.str());
	}

	return queryTokens;
}

QueryProperties QueryPreprocessor::parseQuery(vector<string> queryTokens) {
	return NULL;
}



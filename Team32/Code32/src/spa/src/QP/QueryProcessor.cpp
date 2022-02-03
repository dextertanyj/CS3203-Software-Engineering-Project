#include "QueryProcessor.h"

#include <regex>


string QueryProcessor::processQuery(string query) {

	tokenizeQuery(query);
	parseQuery();
	this->queryResult = QueryEvaluator::executeQuery(this->queryProperties);

	return formatResult();
}

void QueryProcessor::tokenizeQuery(string query) {

	regex invalidCharsRegex = regex(R"([^a-zA-Z0-9 ,\(\);\+\-\*\/%\n])");
	regex queryTokenRegex = regex(R"((Follows|Parents)\*|such that|[a-zA-Z][a-zA-Z0-9]*|[0-9]+|\(|\)|;|\\+|-|\*|\/|%|_|,|\")");

	if (regex_search(query, invalidCharsRegex)) {
		throw QuerySyntaxException("Query included invalid characters");
	}

	auto words_begin = sregex_iterator(
		query.begin(), 
		query.end(), 
		queryTokenRegex
	);
	auto words_end = sregex_iterator();

	for (sregex_iterator i = words_begin; i != words_end; ++i) {
		smatch match = *i;
		this->queryTokens.push_back(match.str());
	}
}

void QueryProcessor::parseQuery() {
	int tokenIndex = 0;
	while (tokenIndex < this->queryTokens.size()) {

	}

	for (int i = 0; i < this->queryTokens.size() - 2; ++i) {

	}
}

string QueryProcessor::formatResult() {
	return NULL;
}


int QueryProcessor::parseDeclaration(int tokenIndex) {
	return NULL;
}

int QueryProcessor::parseSuchThat(int tokenIndex) {
	return NULL;
}

int QueryProcessor::parsePattern(int tokenIndex) {
	return NULL;
}
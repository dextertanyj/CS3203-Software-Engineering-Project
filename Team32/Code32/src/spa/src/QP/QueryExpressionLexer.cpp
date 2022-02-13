#include "QueryExpressionLexer.h"

QueryExpressionLexer::QueryExpressionLexer(vector<string> expressionTokens)
	: expressionTokens(expressionTokens), idx(0) {}

string QueryExpressionLexer::readToken() {
	string token = expressionTokens[idx];
	idx++;
	if (idx >= expressionTokens.size()) {
		throw QueryExpressionException("");
	}
	return token;
}

string QueryExpressionLexer::peekToken() {
	if (idx >= expressionTokens.size()) {
		return 0;
	}

	return expressionTokens[idx];
}
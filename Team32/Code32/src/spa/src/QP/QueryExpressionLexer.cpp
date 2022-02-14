#include "QueryExpressionLexer.h"

QueryExpressionLexer::QueryExpressionLexer(vector<string> expressionTokens)
	: expressionTokens(expressionTokens), idx(0) {}

string QueryExpressionLexer::readToken() {
	if (idx >= expressionTokens.size()) {
		return "";
	}
	string token = expressionTokens[idx];
	idx++;
	return token;
}

string QueryExpressionLexer::peekToken() {
	if (idx >= expressionTokens.size()) {
		return "";
	}

	return expressionTokens[idx];
}
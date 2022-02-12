#include "QueryExpressionLexer.h"

QueryExpressionLexer::QueryExpressionLexer(vector<string> expressionTokens)
	: expressionTokens(expressionTokens), idx(0) {}

string QueryExpressionLexer::readToken() {
	string token = expressionTokens[idx];
	idx++;
	return token;
}

string QueryExpressionLexer::peekToken() {
	return expressionTokens[idx];
}
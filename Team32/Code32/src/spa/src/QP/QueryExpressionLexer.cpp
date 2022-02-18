#include "QueryExpressionLexer.h"

#include <utility>

QueryExpressionLexer::QueryExpressionLexer(vector<string> expression_tokens)
	: expression_tokens(std::move(std::move(expression_tokens))), idx(0) {}

string QueryExpressionLexer::readToken() {
	if (idx >= expression_tokens.size()) {
		return "";
	}
	string token = expression_tokens[idx];
	idx++;
	return token;
}

string QueryExpressionLexer::peekToken() {
	if (idx >= expression_tokens.size()) {
		return "";
	}

	return expression_tokens[idx];
}
#include "QueryExpressionLexer.h"

#include <utility>

using namespace std;

QP::Preprocessor::QueryExpressionLexer::QueryExpressionLexer(vector<string> expression_tokens)
	: expression_tokens(move(expression_tokens)), idx(0) {}

string QP::Preprocessor::QueryExpressionLexer::readToken() {
	if (idx >= expression_tokens.size()) {
		return "";
	}
	string token = expression_tokens[idx];
	idx++;
	return token;
}

string QP::Preprocessor::QueryExpressionLexer::peekToken() {
	if (idx >= expression_tokens.size()) {
		return "";
	}

	return expression_tokens[idx];
}
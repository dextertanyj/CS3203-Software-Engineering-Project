#include "MockLexer.h"

#include <string>
#include <utility>
#include <vector>

using namespace std;

MockLexer::MockLexer(vector<string> tokens) : tokens(std::move(tokens)), index(0) {}

string MockLexer::readToken() {
	if (index >= tokens.size()) {
		return "";
	}
	return tokens.at(index++);
}

string MockLexer::peekToken() {
	if (index >= tokens.size()) {
		return "";
	}
	return tokens.at(index);
}

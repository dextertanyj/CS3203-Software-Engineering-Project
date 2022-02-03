#include "MockLexer.h"

#include <string>
#include <vector>

using namespace std;

MockLexer::MockLexer(vector<string> tokens) : tokens(tokens), index(0) {}

string MockLexer::readToken() {
	return tokens.at(index++);
}

string MockLexer::peekToken() {
	return tokens.at(index);
}

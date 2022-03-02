#include "Lexer.h"

#include <regex>
#include <string>

#include "SP.h"

using namespace std;

regex SP::Lexer::tokenization_regex =  // NOLINT
	regex(R"(([a-zA-Z][0-9a-zA-Z]*|[0-9]+|\{|\}|\(|\)|;|!={0,1}|={1,2}|&&|\|\||>={0,1}|<={0,1}|\+|-|\*|\/|%))");
regex SP::Lexer::validation_regex =  // NOLINT
	regex(R"(([^a-zA-Z0-9\s\{\}\(\);=!&\|><\+\-\*\/%]))");

void SP::Lexer::initialize(string raw_source) {
	this->source = move(raw_source);
	if (regex_search(this->source, Lexer::validation_regex)) {
		throw SP::TokenizationException("Illegal token encountered.");
	}
	this->iterator = sregex_iterator(this->source.begin(), this->source.end(), Lexer::tokenization_regex);
}

bool SP::Lexer::nextToken() {
	if (this->iterator != sregex_iterator()) {
		this->iterator++;
	}
	while (this->iterator != sregex_iterator()) {
		if (!this->iterator->str().empty()) {
			return true;
		}
		this->iterator++;
	}
	return false;
}

string SP::Lexer::readToken() {
	if (this->iterator == sregex_iterator()) {
		return "";
	}
	string token = this->iterator->str();
	nextToken();
	return token;
}

string SP::Lexer::peekToken() {
	if (this->iterator == sregex_iterator()) {
		return "";
	}
	return this->iterator->str();
}

bool SP::Lexer::nextIf(const string& token) {
	if (this->iterator == sregex_iterator()) {
		throw SP::TokenizationException("Unexpected end of sequence.");
	}
	if (this->iterator->str() == token) {
		return nextToken();
	}
	throw SP::TokenizationException("Unexpected token encountered: " + this->iterator->str() + ".");
}

bool SP::Lexer::nextIf(initializer_list<string> tokens) {
	bool last;
	for (const string& token : tokens) {
		last = nextIf(token);
	}
	return last;
}

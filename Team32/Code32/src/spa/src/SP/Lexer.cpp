#include "Lexer.h"
#include "SP.h"

#include <regex>
#include <string>

using namespace std;

regex SP::Lexer::tokenization_regex =
	regex(R"(([a-zA-Z][0-9a-zA-Z]*|[0-9]+|\{|\}|\(|\)|;|!={0,1}|={1,2}|&&|\|\||>={0,1}|<={0,1}|\+|-|\*|\/|%))");  // NOLINT
regex SP::Lexer::validation_regex = regex(R"(([^a-zA-Z0-9 \{\}\(\);=!&\|><\+\-\*\/%\n]))");                           // NOLINT

void SP::Lexer::initialize(string raw_source) {
	this->source = move(raw_source);
	if (regex_search(this->source, Lexer::validation_regex)) {
		throw TokenizationException("Illegal token encountered");
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
	string token = this->iterator->str();
	nextToken();
	return token;
}

string SP::Lexer::peekToken() { return this->iterator->str(); }

bool SP::Lexer::nextIf(const string& token) {
	if (this->iterator->str() == token) {
		return nextToken();
	}
	throw TokenizationException("");
}

bool SP::Lexer::nextIf(initializer_list<string> tokens) {
	bool last;
	for (const string& token : tokens) {
		if (this->iterator->str() != token) {
			throw TokenizationException("");
		}
		last = nextToken();
	}
	return last;
}

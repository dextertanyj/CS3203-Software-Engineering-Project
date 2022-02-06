#include <string>
#include <regex>

#include "Lexer.h"

using namespace std;

regex	Lexer::tokenization_regex = regex(R"(([a-zA-Z][0-9a-zA-Z]*|[0-9]+|\{|\}|\(|\)|;|!={0,1}|={1,2}|&&|\|\||>={0,1}|<={0,1}|\+|-|\*|\/|%))"); // NOLINT
regex	Lexer::validation_regex = regex(R"(([^a-zA-Z0-9 \{\}\(\);=!&\|><\+\-\*\/%\n]))"); // NOLINT

void Lexer::initialize(string raw_source) {
	this->source = move(raw_source);
	if (regex_search(this->source, Lexer::validation_regex)) {
		throw TokenizationException("Illegal token encountered");
	}
	this->iterator = sregex_iterator(this->source.begin(), this->source.end(), Lexer::tokenization_regex);
}

bool Lexer::next_token() {
    if (this->iterator != sregex_iterator()) {
        this->iterator++;
    }
    while(this->iterator != sregex_iterator()) {
        if (!this->iterator->str().empty()) {
            return true;
        }
        this->iterator++;
    }
    return false;
}

string Lexer::read_token() {
	string token = this->iterator->str();
	next_token();
	return token;
}

string Lexer::peek_token() {
	return this->iterator->str();
}

bool Lexer::next_if(const string& token) {
	if (this->iterator->str() == token) {
		return next_token();
	}
	throw TokenizationException("");
}

bool Lexer::next_if(initializer_list<string> tokens) {
	bool last;
	for (const string& token : tokens) {
		if (this->iterator->str() != token) {
			throw TokenizationException("");
		}
		last = next_token();
	}
	return last;
}

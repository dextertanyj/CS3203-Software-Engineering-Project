#include "SP/Lexer.h"

#include "SP/SP.h"

std::regex SP::Lexer::tokenization_regex =  // NOLINT
	std::regex(R"(([a-zA-Z][0-9a-zA-Z]*|[0-9]+|\{|\}|\(|\)|;|!={0,1}|={1,2}|&&|\|\||>={0,1}|<={0,1}|\+|-|\*|\/|%))");
std::regex SP::Lexer::validation_regex =  // NOLINT
	std::regex(R"(([^a-zA-Z0-9\s\{\}\(\);=!&\|><\+\-\*\/%]))");

void SP::Lexer::initialize(std::string raw_source) {
	this->source = move(raw_source);
	if (regex_search(this->source, validation_regex)) {
		throw TokenizationException("Illegal token encountered.");
	}
	this->iterator = std::sregex_iterator(this->source.begin(), this->source.end(), tokenization_regex);
}

std::string SP::Lexer::readToken() {
	if (this->iterator == std::sregex_iterator()) {
		return "";
	}
	std::string token = this->iterator->str();
	nextToken();
	return token;
}

std::string SP::Lexer::peekToken() {
	if (this->iterator == std::sregex_iterator()) {
		return "";
	}
	return this->iterator->str();
}

bool SP::Lexer::nextToken() {
	if (this->iterator != std::sregex_iterator()) {
		this->iterator++;
	}
	while (this->iterator != std::sregex_iterator()) {
		if (!this->iterator->str().empty()) {
			return true;
		}
		this->iterator++;
	}
	return false;
}

bool SP::Lexer::nextIf(const std::string& token) {
	if (this->iterator == std::sregex_iterator()) {
		throw TokenizationException("Unexpected end of sequence.");
	}
	if (this->iterator->str() == token) {
		return nextToken();
	}
	throw TokenizationException("Unexpected token encountered: " + this->iterator->str() + ".");
}

bool SP::Lexer::nextIf(const std::initializer_list<std::string>& tokens) {
	bool last;
	for (const std::string& token : tokens) {
		last = nextIf(token);
	}
	return last;
}

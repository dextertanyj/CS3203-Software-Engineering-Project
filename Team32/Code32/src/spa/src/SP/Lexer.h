#ifndef SPA_LEXER_H
#define SPA_LEXER_H

#include <string>
#include <regex>

using namespace std;

struct TokenizationException : public std::runtime_error {
	using runtime_error::runtime_error;
};

class Lexer {
public:
	void initialize(string);
	bool next_token();
	string read_token();
	string peek_token();
	bool next_if(string&&);

private:
	static regex validation_regex;
	static regex tokenization_regex;

	string source;
	sregex_iterator iterator;
};

#endif //SPA_LEXER_H

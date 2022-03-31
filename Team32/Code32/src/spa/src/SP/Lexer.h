#ifndef SPA_SRC_SP_LEXER_H
#define SPA_SRC_SP_LEXER_H

#include <regex>
#include <string>

#include "Common/ExpressionProcessor/LexerInterface.h"
#include "SP/SP.h"

class SP::Lexer : public Common::ExpressionProcessor::LexerInterface {
public:
	Lexer() = default;
	void initialize(string);
	string readToken() override;
	string peekToken() override;
	bool nextToken();
	bool nextIf(const string& token);
	bool nextIf(const initializer_list<string>& tokens);

private:
	string source;
	sregex_iterator iterator;

	static regex validation_regex;
	static regex tokenization_regex;
};

#endif  // SPA_SRC_SP_LEXER_H

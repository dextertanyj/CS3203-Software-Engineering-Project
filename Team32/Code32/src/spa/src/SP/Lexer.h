#ifndef SPA_SRC_SP_LEXER_H
#define SPA_SRC_SP_LEXER_H

#include <regex>
#include <string>

#include "Common/ExpressionProcessor//LexerInterface.h"
#include "SP/SP.h"

using namespace std;

class SP::Lexer : public Common::ExpressionProcessor::LexerInterface {
public:
	void initialize(string);
	bool nextToken();
	string readToken() override;
	string peekToken() override;
	bool nextIf(const string& token);
	bool nextIf(initializer_list<string> tokens);

private:
	static regex validation_regex;
	static regex tokenization_regex;

	string source;
	sregex_iterator iterator;
};

#endif  // SPA_SRC_SP_LEXER_H

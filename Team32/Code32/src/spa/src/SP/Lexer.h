#ifndef SPA_SRC_SP_LEXER_H
#define SPA_SRC_SP_LEXER_H

#include <regex>
#include <string>

#include "Common/ExpressionProcessor/LexerInterface.h"
#include "SP/SP.h"

class SP::Lexer : public Common::EP::LexerInterface {
public:
	Lexer() = default;
	void initialize(std::string raw_source);
	std::string readToken() override;
	std::string peekToken() override;
	bool nextIf(const std::string& token);
	bool nextIf(const std::initializer_list<std::string>& tokens);

private:
	static std::regex validation_regex;
	static std::regex tokenization_regex;

	bool nextToken();

	std::string source;
	std::sregex_iterator iterator;
};

#endif  // SPA_SRC_SP_LEXER_H

#pragma once

#include <string>

#include "SP/Lexer.h"

using namespace std;

class Parser {
public:
	Parser();
	bool parse(string source);

private:
	Lexer lex;
	int statement_count;
};

#pragma once

#include <string>

#include "SP/Lexer.h"
#include "SP/Node/ProgramNode.h"
#include "SP/SP.h"

using namespace std;

class SP::Parser {
public:
	explicit Parser(SP::Lexer lex);
	unique_ptr<ProgramNode> parse(string source);

private:
	SP::Lexer lex;
	int statement_count;
};

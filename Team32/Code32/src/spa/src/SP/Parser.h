#pragma once

#include <string>

#include "SP/SP.h"
#include "SP/Lexer.h"
#include "SP/Node/ProgramNode.h"

using namespace std;

class SP::Parser {
public:
	Parser();
	unique_ptr<ProgramNode> parse(string source);

private:
	SP::Lexer lex;
	int statement_count;
};

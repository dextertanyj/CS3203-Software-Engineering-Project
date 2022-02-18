#pragma once

#include <memory>
#include <string>

#include "SP/Lexer.h"
#include "SP/Node/ProgramNode.h"
#include "SP/SP.h"

using namespace std;

class SP::Parser {
public:
	explicit Parser(SP::Lexer lex);
	unique_ptr<Node::ProgramNode> parse(string source);

private:
	SP::Lexer lex;
	StmtRef statement_count;
};

#ifndef SPA_SRC_SP_PARSER_H
#define SPA_SRC_SP_PARSER_H

#define STARTING_STATEMENT_NUMBER 1

#include <memory>
#include <string>

#include "SP/Lexer.h"
#include "SP/Node/ProgramNode.h"
#include "SP/SP.h"
#include "Common/TypeDefs.h"

class SP::Parser {
public:
	Parser() = default;
	std::unique_ptr<Node::ProgramNode> parse(string source);

private:
	SP::Lexer lex;
	StmtRef statement_count = STARTING_STATEMENT_NUMBER;
};

#endif

#include "SP/Parser.h"

#define STARTING_STATEMENT_NUMBER 1

using namespace std;

SP::Parser::Parser(SP::Lexer lex) : lex(std::move(lex)), statement_count(1) {}

unique_ptr<SP::Node::ProgramNode> SP::Parser::parse(string source) {
	this->lex.initialize(std::move(source));
	this->statement_count = STARTING_STATEMENT_NUMBER;
	return Node::ProgramNode::parseProgram(lex, this->statement_count);
}
